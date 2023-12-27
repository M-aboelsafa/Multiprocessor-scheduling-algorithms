# Multiprocessor System Process Scheduler Simulator

This project is a process scheduler simulator for a multiprocessor system, implementing various scheduling policies such as FCFS, SJF, STCF, RR, priority, MLFQ, and stride. The simulator allows users to analyze the performance of each policy by reporting statistics such as turnaround time. Additionally, the system considers the preference of processors for specific jobs and provides a clear representation of the scheduler's operation, such as a Gantt chart.

## Table of Contents

- [Introduction](#introduction)
- [Implemented Scheduling Policies](#implemented-scheduling-policies)
- [Performance Statistics](#performance-statistics)
- [Interactive Features](#interactive-features)
- [Test Set of Processes](#test-set-of-processes)
- [How to Run](#how-to-run)
- [Contributing](#contributing)

## Introduction

The Process Scheduler Simulator for Multiprocessor Systems is a comprehensive tool designed to emulate and evaluate different scheduling policies within a multiprocessor environment. The primary objective of this project is to offer a versatile platform that allows users to simulate, analyze, and compare the performance of distinct scheduling algorithms, including First-Come, First-Served (FCFS), Shortest Job First (SJF), Shortest Time-to-Completion First (STCF), Round Robin (RR), Priority, Multilevel Feedback Queue (MLFQ), and Stride scheduling.

## Implemented Scheduling Policies

List and briefly describe the implemented scheduling policies in the simulator.

1. **FFirst-Come, First-Served (FCFS)**: Processes are executed in the order they arrive.
2. **Shortest Job First (SJF)**: Prioritizes the shortest burst time processes.
3. **Shortest Time-to-Completion First (STCF)**: Selects processes based on remaining time to completion.
4. **Round Robin (RR)**: Time-sliced scheduling for fair execution among processes.
5. **Priority Scheduling**: Assigns priority levels to processes for execution.
6. **Multilevel Feedback Queue (MLFQ)**: Implements multiple queues with varied priorities.
7. **Stride Scheduling**: Utilizes proportional allocation of CPU time based on stride values.


Include links to relevant documentation or external resources for each scheduling policy.

## Performance Statistics

•    Generates processor-wise execution snapshots using a two-dimensional vector (output) to represent the progress of each process over time.
•    Calculates process statistics such as turnaround time and completion time for each process.
## Interactive Features

•    Interaction primarily via the command-line interface for simplicity.
•    Provides user prompts to input the number of processes and generates sample processes with randomized attributes within a file and the system takes the input from the file itself.
•    Initiates the Scheduler chosen policy and displays the output, including processor-wise execution snapshots.

## Test Set of Processes
[Test case](https://github.com/M-aboelsafa/operating-system-scheduling-algorithms/blob/main/input.txt)
![image](https://github.com/M-aboelsafa/operating-system-scheduling-algorithms/assets/78052557/36179e43-c3f5-490d-8ae4-da613b3af433)

## How to Run

to run the program you need to specify the following in the input file:
number of processes
process name
process phases number
process arrival time 
process phases description (3 EXE, 4 I/O) for each phase

```bash
# Example command to run the simulator
g++ Scheduler.h -o Scheduler.exe
g++ *.cpp -o out
./out
```

## contributing
Mohamed Yousef Helmy
Mohamed Ahmed Rabea
Mostafa Wael Hussien
Ahmed Sayed Mohamed
Ali Abdelmenaim Mohamed
