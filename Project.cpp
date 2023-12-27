#include "scheduler.h"

using namespace std;

// priority_queue<process,compare>

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
        processes[i].tickets = calculateTickets(processes[i]);
        processes[i].pass_value = 0;
        processes[i].time_consumed = 0;
        processes[i].stride = MAX_STRIDE / processes[i].tickets;
        // processes[i].current_brust_time = processes[i].phases[0].first;
    }
}
void Input_Priority(int n, process processes[])
{
    string y;
    int x;
    for (int i = 0; i < n; i++)
    {
        cin >> processes[i].process_name;
        cin >> processes[i].n_phases;
        cin >> processes[i].arrive_time;
        cin >> processes[i].priority;
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
    if (scheduler == "Priority")
    {
        Priority(n, processes);
    }
    if (scheduler == "Stride")
    {
        strideScheduler(n, processes);
    }
}
int main()
{

    freopen("input.txt", "r", stdin);

    string scheduler;
    cin >> scheduler;
    int n;
    cin >> n;
    process processes[n];
    if (scheduler == "Priority")
        Input_Priority(n, processes);
    else
        Input(n, processes);
    choose_scheduler(n, scheduler, processes);
}