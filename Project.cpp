#include <bits/stdc++.h>
#define el '\n'
using namespace std;
int N = 100000;
vector<vector<string>> output(N, vector<string>(4));

// priority_queue<process,compare>
struct process
{
    string process_name;
    int process_idx; // index in the array
    int arrive_time;
    int turn_around_time;
    int complete_time;
    int time_consumed; // current phase take kam second
    int n_phases;      //  kam phase
    int phase_idx;
    int last_processor;             // last processor work on
    vector<pair<int, bool>> phases; //
    int state;                      // current state
    int priority;
    int current_brust_time;
};
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
void round_robin(int n, process processes[])
{
    queue<int> my_queue;
    int finised_processes = 0, WillGoToTheQ[4], processor[4], p, i = 0, time_slice = 10;
    memset(processor, -1, sizeof(processor));
    memset(WillGoToTheQ, -1, sizeof(WillGoToTheQ));
    for (int t = 0;; ++t)
    {
        // push in queue
        int idx_p = 0;

        for (idx_p = 0; idx_p < 4 && i < n && processes[i].arrive_time <= t; ++idx_p)
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
        while (i < n && processes[i].arrive_time <= t)
        {
            my_queue.push(i);

            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            i++;
        }

        // dowork in processor
        for (idx_p = 0; idx_p < 4; ++idx_p)
        {
            // idle
            if (processor[idx_p] == -1)
            {
                output[t][idx_p] = "i";
                if (!my_queue.empty())
                {
                    WillGoToTheQ[idx_p] = my_queue.front(); /////law zawd t7t
                    my_queue.pop();
                }
                continue;
            }

            // do the work for one unit of time
            p = processor[idx_p];
            output[t][idx_p] = processes[p].process_name;
            processes[p].time_consumed++;

            // check to remove the process from the processor
            if (processes[p].time_consumed % time_slice == 0 || processes[p].time_consumed == processes[p].current_brust_time) // remove from processor
            {
                if (processes[p].time_consumed == processes[p].current_brust_time) // removed after finishing EXE
                {
                    processes[p].phase_idx++;
                    processes[p].time_consumed = 0;
                    if (processes[p].phase_idx < processes[p].n_phases)
                    {
                        processes[p].state = processes[p].phases[processes[p].phase_idx].second;
                        processes[p].current_brust_time = processes[p].phases[processes[p].phase_idx].first;
                    }
                    else
                    {
                        finised_processes++;
                        processes[p].state = 2;
                        processes[p].complete_time = t;
                        processes[p].turn_around_time = t - processes[p].arrive_time;
                    }
                }
                else // removed after time slice
                {
                    my_queue.push(p);
                }

                // add process for the processor
                if (!my_queue.empty())
                {
                    WillGoToTheQ[idx_p] = my_queue.front();
                    my_queue.pop();
                }
                processor[idx_p] = -1;
            }
        }
        match_prefrences(processor, WillGoToTheQ, processes);
        // do IO
        IO_handler(i, processes, my_queue, finised_processes, t);

        // 5lsna
        if (finised_processes == n)
        {
            print(t);
            return;
        }
    }
}
void Input(int n, process processes[])
{
    string y;
    int x;
    for (int i = 0; i < n; i++)
    {
        cin >> processes[i].process_name;
        cin >> processes[i].n_phases;
        cin >> processes[i].arrive_time;
        processes[i].phases.resize(processes[i].n_phases);
        for (int j = 0; j < processes[i].n_phases; j++)
        {
            cin >> processes[i].phases[j].first;
            cin >> y;
            if (y == "I/O")
            {
                processes[i].phases[j].second = 0;
            }
            if (y == "EXE")
            {
                processes[i].phases[j].second = 1;
            }
        }
        processes[i].last_processor = -1;
        processes[i].phase_idx = 0;
        processes[i].time_consumed = 0;
        processes[i].process_idx = i;
        // processes[i].current_brust_time = processes[i].phases[0].first;
    }
}

void choose_scheduler(int n, string scheduler, process processes[])
{
    if (scheduler == "RR")
    {
        round_robin(n, processes);
    }
}
int main()
{
    string scheduler;
    cin >> scheduler;
    int n;
    cin >> n;
    process processes[n];
    Input(n, processes);
    choose_scheduler(n, scheduler, processes);
}
