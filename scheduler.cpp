#include "scheduler.h"

vector<vector<string>> output(N, vector<string>(4));


bool comparedByBurst(const process &p1, const process &p2)
{
    if (p1.arrive_time == p2.arrive_time)
        return p1.phases[0].first < p2.phases[0].first;
    else
        return p1.arrive_time < p2.arrive_time;
}

void print(int endtime)
{
    for (int j = 0; j < 4; j++) // no of processors
    {
        cout << "processor " << j + 1 << " : ";
        for (int i = 0; i < endtime; i++) // all seconds
        {
            cout << output[i][j] << "|";
        }
        cout << output[endtime][j] << "\n";
    }
}
void IO_handler(int n, process processes[], queue<int> &my_queue, int &finised_processes, int t)
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].state == 0) // state IO
        {
            // cout << "IO handler " << i << "\n";
            processes[i].time_consumed++;
            if (processes[i].time_consumed == processes[i].current_brust_time) // IO Finished
            {
                processes[i].time_consumed = 0;
                processes[i].phase_idx++;
                if (processes[i].phase_idx < processes[i].n_phases)
                {
                    processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                    processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                    if (processes[i].state)
                    {
                        // cout << "PUSH\n";
                        my_queue.push(i);
                    }
                }
                else
                {
                    // cout << "DONE\n";
                    processes[i].state = 2;
                    (finised_processes)++;
                    processes[i].complete_time = t;
                    processes[i].turn_around_time = t - processes[i].arrive_time;
                }
            }
        }
    }
}
void IO_handler_MLFQ(int n, process processes[], queue<int> my_queue[], int &finised_processes, int t)
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].state == 0) // state IO
        {
            // cout << "IO handler " << i << "\n";
            processes[i].time_consumed++;
            if (processes[i].time_consumed == processes[i].current_brust_time) // IO Finished
            {
                processes[i].time_consumed = 0;
                processes[i].phase_idx++;
                if (processes[i].phase_idx < processes[i].n_phases)
                {
                    processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                    processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                    if (processes[i].state)
                    {
                        // cout << "PUSH\n";
                        my_queue[processes[i].priority].push(i);
                    }
                }
                else
                {
                    // cout << "DONE\n";
                    processes[i].state = 2;
                    (finised_processes)++;
                    processes[i].complete_time = t;
                    processes[i].turn_around_time = t - processes[i].arrive_time;
                }
            }
        }
    }
}
void IO_handler_bypq(int n, process processes[], priority_queue<int, vector<int>, CompareProcess> &my_queue, int &finised_processes, int t)
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].state == 0) // state IO
        {
            // cout << "IO handler " << i << "\n";
            processes[i].time_consumed++;
            if (processes[i].time_consumed == processes[i].current_brust_time) // IO Finished
            {
                processes[i].time_consumed = 0;
                processes[i].phase_idx++;
                if (processes[i].phase_idx < processes[i].n_phases)
                {
                    processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                    processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                    if (processes[i].state)
                    {
                        // cout << "PUSH\n";
                        my_queue.push(i);
                    }
                }
                else
                {
                    // cout << "DONE\n";
                    processes[i].state = 2;
                    (finised_processes)++;
                    processes[i].complete_time = t;
                    processes[i].turn_around_time = t - processes[i].arrive_time;
                }
            }
        }
    }
}
void match_prefrences(int processor[], int WillGoToTheQ[], process processes[])
{
    // First loop: Assign processes to processors if the processor is available
    for (int i = 0; i < 4; i++)
    {
        if (WillGoToTheQ[i] != -1 && processes[WillGoToTheQ[i]].last_processor != -1 && processor[processes[WillGoToTheQ[i]].last_processor] == -1)
        {
            processor[processes[WillGoToTheQ[i]].last_processor] = WillGoToTheQ[i];
            WillGoToTheQ[i] = -1;
        }
    }

    // Second loop: Assign processes to the first available processor
    for (int i = 0; i < 4; i++)
    {
        if (WillGoToTheQ[i] != -1)
        {
            for (int j = 0; j < 4; j++)
            {
                if (processor[j] == -1)
                {
                    processor[j] = WillGoToTheQ[i];
                    processes[WillGoToTheQ[i]].last_processor = j;
                    WillGoToTheQ[i] = -1;
                    break; // Break out of the loop after assigning the process
                }
            }
        }
    }
}
