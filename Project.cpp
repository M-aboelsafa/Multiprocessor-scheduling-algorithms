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
    int qunta; // to calculate time in MLFQ
};
struct CompareProcess
{
    const process *data;
    CompareProcess(const process *d) : data(d) {}
    bool operator()(int a, int b) const
    {
        return (data[a].current_brust_time - data[a].time_consumed) > (data[b].current_brust_time - data[a].time_consumed);
    }
};
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
void changeThePriorty(int n, process processes[])
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].state == 2)
            continue;
        processes[i].priority = 0;
        processes[i].qunta = 0;
    }
}

void MLFQ(int n, process processes[])
{
    int time_to_moveup = 10, finised_processes = 0, MAX_Q = 4, current_queue = 0, WillGoToTheQ[4], processor[4], p, i = 0, time_slice = 10;

    queue<int> my_queue[MAX_Q];
    memset(processor, -1, sizeof(processor));
    memset(WillGoToTheQ, -1, sizeof(WillGoToTheQ));
    for (int t = 0;; ++t)
    {
        // push in queue & CPU
        int idx_p = 0;

        for (idx_p = 0; idx_p < 4 && i < n && processes[i].arrive_time <= t; ++idx_p)
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = i;
                processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                processes[i].last_processor = idx_p;
                processes[i].priority = 0;
                i++;
            }
        }
        while (i < n && processes[i].arrive_time <= t)
        {
            processes[i].priority = 0;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            bool flag = false;
            for (idx_p = 0; idx_p < 4; idx_p++)
            {
                if (processor[idx_p] == -1)
                    continue;

                if (processes[processor[idx_p]].priority > 0)
                {
                    my_queue[processes[processor[idx_p]].priority].push(processor[idx_p]);
                    processor[idx_p] = i;
                    flag = true;
                    break;
                }
            }
            if (flag == false)
                my_queue[0].push(i);
            i++;
        }

        // names in the processor in each unit of time
        /*
        cout << t << el;
        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1)
                continue;

            cout << processes[processor[idx_p]].priority << " " << processor[idx_p] << " " << processes[processor[idx_p]].process_name << " " << processes[processor[idx_p]].qunta << " " << processes[processor[idx_p]].time_consumed << " " << (1 << (processes[p].priority + 1)) << el;
        }
        */

        // dowork in processor
        for (idx_p = 0; idx_p < 4; ++idx_p)
        {
            // idle
            if (processor[idx_p] == -1)
            {
                output[t][idx_p] = "i";
                continue;
            }

            // do the work for one unit of time
            p = processor[idx_p];
            output[t][idx_p] = processes[p].process_name;
            processes[p].qunta++;
            processes[p].time_consumed++;

            // check to remove the process from the processor
            if (processes[p].qunta % (1 << (processes[p].priority + 1)) == 0 || processes[p].time_consumed == processes[p].current_brust_time) // remove from processor
            {

                if (processes[p].qunta % (1 << (processes[p].priority + 1)) == 0)
                {
                    // move to the next queue level
                    processes[p].priority++;
                    processes[p].qunta = 0;
                }
                if (processes[p].time_consumed == processes[p].current_brust_time) // removed after finishing EXE
                {
                    // move to the next phase
                    processes[p].phase_idx++;
                    processes[p].time_consumed = 0;
                    if (processes[p].phase_idx < processes[p].n_phases)
                    {
                        processes[p].state = processes[p].phases[processes[p].phase_idx].second;
                        processes[p].current_brust_time = processes[p].phases[processes[p].phase_idx].first;
                    }
                    else // the process finished
                    {
                        finised_processes++;
                        processes[p].state = 2;
                        processes[p].complete_time = t;
                        processes[p].turn_around_time = t - processes[p].arrive_time;
                    }
                }
                else // removed after time slice
                {
                    my_queue[processes[p].priority].push(p);
                }

                processor[idx_p] = -1;
            }
        }

        // add process for the processor

        current_queue = 0;
        for (idx_p = 0; idx_p < 4; ++idx_p) // add for the idle processors
        {
            if (processor[idx_p] != -1)
                continue;
            while (current_queue < MAX_Q && my_queue[current_queue].empty())
            {
                current_queue++;
            }
            if (current_queue < MAX_Q && !my_queue[current_queue].empty())
            {
                WillGoToTheQ[idx_p] = my_queue[current_queue].front();
                my_queue[current_queue].pop();
            }
        }
        while (1) // add for the processors with lower priorites
        {
            bool flag = false;
            while (current_queue < MAX_Q && my_queue[current_queue].empty())
            {
                current_queue++;
            }
            if (current_queue < MAX_Q && !my_queue[current_queue].empty())
            {
                // cout << processes[my_queue[current_queue].front()].process_name << el;
                for (idx_p = 0; idx_p < 4; idx_p++)
                {
                    if (processor[idx_p] == -1)
                        continue;

                    // cout << processes[processor[idx_p]].priority << " " << processor[idx_p] << " " << processes[processor[idx_p]].process_name << " - " << current_queue << " " << my_queue[current_queue].front() << el;
                    if (processes[processor[idx_p]].priority > current_queue)
                    {
                        my_queue[processes[processor[idx_p]].priority].push(processor[idx_p]);
                        processor[idx_p] = -1;
                        WillGoToTheQ[idx_p] = my_queue[current_queue].front();
                        my_queue[current_queue].pop();
                        flag = true;
                        break;
                    }
                }
            }
            if (!flag)
                break;
        }
        // Match each process with its preferable queue
        match_prefrences(processor, WillGoToTheQ, processes);
        //  Do IO
        IO_handler_MLFQ(i, processes, my_queue, finised_processes, t);

        // THE FUNCTION FINISHED SUCCESFULLY
        if (finised_processes == n)
        {
            print(t);
            return;
        }

        if (t != 0 && t % time_to_moveup == 0)
        {
            changeThePriorty(i, processes);
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
void SJF(int size, process processes[])
{
    sort(processes, processes + size, comparedByBurst);
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    CompareProcess cmp(processes);
    priority_queue<int, vector<int>, CompareProcess> waiting(cmp);
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
            print(t);
            return;
        }
    }
}
void STCF(int size, process processes[])
{
    sort(processes, processes + size, comparedByBurst);
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    CompareProcess cmp(processes);
    priority_queue<int, vector<int>, CompareProcess> waiting(cmp);
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
            /* else // some process is at cpu right now
              {
                  int waiting_process_current_time_left= processes[i].phases[processes[i].phase_idx].first;
                  int running_process_time_left= processes[processor[idx_p]].current_brust_time - processes[processor[idx_p]].time_consumed;
                  if(waiting_process_current_time_left < running_process_time_left)
                  {
                      waiting.push(processor[idx_p]);
                      processes[i].current_brust_time=processes[i].phases[processes[i].phase_idx].first;
                      processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                      processes[i].last_processor = idx_p;
                      processor[idx_p]=i;
                  }
                  else
                  {
                      processor[idx_p] = i;
                      processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                      processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                      processes[i].last_processor = idx_p;
                  }
                  i++;
              }*/
        }
        int replace = 0;
        while (i < size && processes[i].arrive_time <= t) // processes arrived but there is no place at cpu so it will go in waiting queue
        {

            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            waiting.push(i);
            i++;
            for (int q = 0; q < 4; q++)
            {
                if (processor[q] != -1)
                {
                    waiting.push(processor[q]);
                }
            }
            for (int q = 0; q < 4; q++)
            {
                processor[q] = waiting.top();
                waiting.pop();
            }
        }

        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // means that , that processor is idle
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
            print(t);
            return;
        }
    }
}
void FCFS (int size , process processes [])
{
    int finished_processes=0,processor[4],WillGoToTheQ[4],i=0,pr;
    memset(processor,-1,sizeof processor);
    memset(WillGoToTheQ,-1,sizeof WillGoToTheQ);
    queue<int>waiting;
    for(int t=0;;t++)
    {
        int idx_p=0;
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
        while(i<size && processes[i].arrive_time <=t ) // processes arrived but there is no place at cpu so it will go in waiting queue
        {
            waiting.push(i);
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            i++;
        }
        for(idx_p =0 ;idx_p<4;idx_p++)
        {
            if(processor[idx_p]==-1) // means that , that processor is ideal
            {
                output[t][idx_p]="i";
                if(!waiting.empty())
                {
                    WillGoToTheQ[idx_p]=waiting.front();
                    waiting.pop();
                }
                continue;
            }
            pr = processor[idx_p];  // index of the process in the processor
            output[t][idx_p]=processes[pr].process_name;
            processes[pr].time_consumed++;
            if(processes[pr].time_consumed == processes[pr].current_brust_time) // proceses finished exec
            {
                processes[pr].phase_idx++;
                processes[pr].time_consumed=0;
                if(processes[pr].phase_idx < processes[pr].n_phases) // still want more ?
                {
                    processes[pr].state = processes[pr].phases[processes[pr].phase_idx].second;
                    processes[pr].current_brust_time = processes[pr].phases[processes[pr].phase_idx].first;
                }
                else //process finished
                {
                    finished_processes++;
                    processes[pr].state=2;
                    processes[pr].complete_time=t;
                    processes[pr].turn_around_time = t - processes[pr].arrive_time;
                }
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.front();
                    waiting.pop();
                }
                processor[idx_p]=-1;
            }

        }
        match_prefrences(processor,WillGoToTheQ,processes);
        IO_handler(size,processes,waiting,finished_processes,t);

        if (finished_processes == size)
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
        processes[i].state = 1;
        processes[i].qunta = 0;
        // processes[i].current_brust_time = processes[i].phases[0].first;
    }
}

void choose_scheduler(int n, string scheduler, process processes[])
{
    if (scheduler == "RR")
    {
        round_robin(n, processes);
    }
    if (scheduler == "MLFQ")
    {
        MLFQ(n, processes);
    }
    if (scheduler == "SJF")
    {
        SJF(n, processes);
    }
    if (scheduler == "STCF")
    {
        STCF(n, processes);
    }
    if (scheduler == "FCFS")
    {
        FCFS(n, processes);
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
