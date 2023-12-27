#include "scheduler.h"

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
            print(t, processes, n);
            return;
        }

        if (t != 0 && t % time_to_moveup == 0)
        {
            changeThePriorty(i, processes);
        }
    }
}