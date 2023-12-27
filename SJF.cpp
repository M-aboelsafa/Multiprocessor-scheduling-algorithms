#include "scheduler.h"

void SJF(int size, process processes[])
{
    sort(processes, processes + size, comparedByBurst);
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    CompareProcess cmp(processes);
    priority_queue<int, vector<int>, CompareProcess> waiting(cmp);  //O(logn)
    for (int t = 0;; t++)
    {
        int idx_p = 0;
        for (idx_p = 0; idx_p < 4 && i < size && processes[i].arrive_time <= t; ++idx_p) // if processor is ideal and processes
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = i;
                processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                processes[i].last_processor = idx_p;
                i++;
            }
        }
        while (i < size && processes[i].arrive_time <= t) // processes arrived but there is no place at cpu so it will go in waiting queue
        {
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            waiting.push(i);
            i++;
        }
        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // means that , that processor is ideal
            {
                output[t][idx_p] = "i";
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.top();
                    waiting.pop();
                }
                continue;
            }
            pr = processor[idx_p]; // index of the process in the processor
            output[t][idx_p] = processes[pr].process_name;
            processes[pr].time_consumed++;
            if (processes[pr].time_consumed == processes[pr].current_brust_time) // proceses finished exec
            {
                processes[pr].phase_idx++;
                processes[pr].time_consumed = 0;
                if (processes[pr].phase_idx < processes[pr].n_phases) // still want more ?
                {
                    processes[pr].state = processes[pr].phases[processes[pr].phase_idx].second;
                    processes[pr].current_brust_time = processes[pr].phases[processes[pr].phase_idx].first;
                }
                else // process finished
                {
                    finished_processes++;
                    processes[pr].state = 2;
                    processes[pr].complete_time = t;
                    processes[pr].turn_around_time = t - processes[pr].arrive_time;
                }
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.top();
                    waiting.pop();
                }
                processor[idx_p] = -1;
            }
        }
        match_prefrences(processor, WillGoToTheQ, processes);
        IO_handler_bypq(size, processes, waiting, finished_processes, t);

        if (finished_processes == size)
        {
            print(t, processes, size);
            return;
        }
    }
}
