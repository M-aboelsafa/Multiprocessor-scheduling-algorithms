#include <iostream>
using namespace std;
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

void Input(int n, process processes[])
{
    string y;
    int x;
    for (int i = 0; i < n; i++)
    {
        cout<<"enter name of process\n";
        cin >> processes[i].process_name;
        cout<<"enter number of phases\n";
        cin >> processes[i].n_phases;
        cout<<"enter the arrive time\n";
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
        processes[i].complete_time = 0;
        processes[i].turn_around_time = 0;
        processes[i].state=-1;
        // processes[i].current_brust_time = processes[i].phases[0].first;
    }
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

void match_prefrences(int processor[], int WillGoToTheQ[], process processes[])
{
    // First loop: Assign processes to processors if the processor is available
    for (int i = 0; i < 4; i++)
    {
        if (WillGoToTheQ[i] != -1 && processor[processes[WillGoToTheQ[i]].last_processor] == -1)
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
void sort_queue(queue<int>&ss,process processes [])
{
    vector<pair<int,int>>v;
    v.resize(ss.size());
    int idx=0;
    while (!ss.empty()) {
        v[idx++]={(processes[ss.front()].current_brust_time - processes[ss.front()].time_consumed),ss.front()};
        ss.pop();
    }
    sort(v.begin(),v.end());
    for(int i=0;i<v.size();i++)
    {
        ss.push(v[i].second);
    }
}
bool comparedByBurst(const process& p1,const process& p2)
{
    if(p1.arrive_time == p2.arrive_time)
        return p1.phases[0].first <p2.phases[0].first;
    else
        return p1.arrive_time < p2.arrive_time;
}
void SJF(int size,process processes [] )
{
    sort(processes,processes+size, comparedByBurst);
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
        if(!waiting.empty())
            sort_queue(waiting,processes);
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
void replace_process(int idx_p,int processor [],queue<int>&my_queue,process processes[])
{
   int waiting_process_current_time_left= processes[my_queue.front()].current_brust_time -processes[my_queue.front()].time_consumed;
   int running_process_time_left= processes[processor[idx_p]].current_brust_time - processes[processor[idx_p]].time_consumed;
   if(waiting_process_current_time_left < running_process_time_left) // u will switch between process on cpu with process on waiting
   {
       my_queue.push(processor[idx_p]);
       processor[idx_p]=my_queue.front();
       my_queue.pop();
   }
}
void STCF(int size,process processes [])
{
    sort(processes,processes+size, comparedByBurst);
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
//        if(!waiting.empty())
//            sort_queue(waiting,processes);
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
            replace_process(idx_p,processor,waiting,processes);
            sort_queue(waiting,processes);
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
int main() {

    int n;
    cout<<"enter number of process\n";
    cin >> n;
    process processes[n];
    Input(n, processes);
    STCF(n,processes);
    return 0;

}
