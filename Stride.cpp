#include "scheduler.h"

int calculateTickets(process p)
{
    int time = 0;
    int execution_requests = 0;
    int io_requests = 0;
    for (int j = 0; j < p.n_phases; ++j)
    {
        time += p.phases[j].first;
        if (p.phases[j].second == 1)
        {
            execution_requests++;
        }
        else
        {
            io_requests++;
        }
    }
    return time + execution_requests * 20 + io_requests * 30;
}

void strideScheduler(int n, process processes[])
{
    queue<int> waiting, q, q2;
    int finished_processes = 0, WillGoToTheQ[4], processor[4], p_num, i = 0;
    memset(processor, -1, sizeof(processor));
    memset(WillGoToTheQ, -1, sizeof(WillGoToTheQ));

    for (int t = 0;; ++t)
    {
        bool completed = true;
        int idx_p = 0;
        vector<int> indices(n);
        if (i <= 4)
        {
            iota(indices.begin(), indices.end(), 0); // Fill with 0 to n-1

            // Sort the indices based on pass_value of processes
            sort(indices.begin(), indices.end(), [&](int a, int b)
                 { return processes[a].arrive_time < processes[b].arrive_time; });

            // Push the sorted indices into the queue
            for (int idx : indices)
            {
                if (processes[idx].state != 2)
                    q.push(processes[idx].process_idx);
            }
        }
        else
        {
            iota(indices.begin(), indices.end(), 0); // Fill with 0 to n-1

            // Sort the indices based on pass_value of processes
            sort(indices.begin(), indices.end(), [&](int a, int b)
                 { return processes[a].pass_value < processes[b].pass_value; });

            // Push the sorted indices into the queue
            for (int idx : indices)
            {
                if (processes[idx].state != 2)
                    q.push(processes[idx].process_idx);
            }
        }

        for (idx_p = 0; idx_p < 4 && !q.empty() && processes[q.front()].arrive_time <= t; idx_p++)
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = q.front();
                processes[q.front()].current_brust_time = processes[q.front()].phases[processes[q.front()].phase_idx].first;
                processes[q.front()].state = processes[q.front()].phases[processes[q.front()].phase_idx].second;
                processes[q.front()].last_processor = idx_p;
                i++;
                q.pop();
            }
        }

        while (!q.empty() && processes[q.front()].arrive_time <= t)
        {
            waiting.push(q.front());
            processes[q.front()].current_brust_time = processes[q.front()].phases[processes[q.front()].phase_idx].first;
            processes[q.front()].state = processes[q.front()].phases[processes[q.front()].phase_idx].second;
            q.pop();
        }
        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // means that , that processor is idle
            {
                output[t][idx_p] = "i";
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.front();
                    waiting.pop();
                }
                continue;
            }
            p_num = processor[idx_p]; // index of the process in the processor
            output[t][idx_p] = processes[p_num].process_name;
            processes[p_num].pass_value += processes[p_num].stride;
            processes[p_num].time_consumed++;
            if (processes[p_num].time_consumed == processes[p_num].current_brust_time) // proceses finished exec
            {
                processes[p_num].phase_idx++;
                processes[p_num].time_consumed = 0;
                if (processes[p_num].phase_idx < processes[p_num].n_phases) // still want more ?
                {
                    processes[p_num].state = processes[p_num].phases[processes[p_num].phase_idx].second;
                    processes[p_num].current_brust_time = processes[p_num].phases[processes[p_num].phase_idx].first;
                }
                else
                {
                    finished_processes++;
                    processes[p_num].state = 2;
                    processes[p_num].complete_time = t;
                    processes[p_num].turn_around_time = t - processes[p_num].arrive_time;
                }
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.front();
                    waiting.pop();
                }
                processor[idx_p] = -1;
            }
        }
        while (!q.empty())
        {
            q.pop();
        }
        match_prefrences(processor, WillGoToTheQ, processes);
        IO_handler(n, processes, waiting, finished_processes, t);

        if (completed)
        {
            print(t, processes, n);
            return;
        }
    }
}