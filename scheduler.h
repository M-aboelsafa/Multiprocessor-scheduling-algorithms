#include <bits/stdc++.h>
using namespace std;
#define el '\n'
#define N (int)100000
extern std::vector<std::vector<std::string>> output;

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
bool comparedByBurst(const process &p1, const process &p2);
void print(int endtime);
void IO_handler(int n, process processes[], queue<int> &my_queue, int &finised_processes, int t);
void IO_handler_MLFQ(int n, process processes[], queue<int> my_queue[], int &finised_processes, int t);
void IO_handler_bypq(int n, process processes[], priority_queue<int, vector<int>, CompareProcess> &my_queue, int &finised_processes, int t);
void match_prefrences(int processor[], int WillGoToTheQ[], process processes[]);

void changeThePriorty(int n, process processes[]);
void MLFQ(int n, process processes[]);
void round_robin(int n, process processes[]);
void SJF(int size, process processes[]);
void STCF(int size, process processes[]);
void FCFS(int size, process processes[]);