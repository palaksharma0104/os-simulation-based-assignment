#include <stdio.h>
#include <stdlib.h> // for rand() and srand()
#include <time.h>   // for seeding the random number generator

struct Process
{
    int process_no;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int priority;
    int temp_priority;
};

struct Process generate_random_process(int process_number)
{
    struct Process p;
    p.process_no = process_number;

    p.arrival_time = rand() % 10;   // Generate random arrival time between 0 and 9
    p.burst_time = 1 + rand() % 10; // Generate random burst time between 1 and 10
    p.remaining_time = p.burst_time;

    p.priority = rand() % 10; // Generate random priority between 0 and 9
    p.temp_priority = p.priority;

    return p;
}

int main(void)
{
    int i, num_processes, current_time, remaining_processes, max_priority, max_priority_index;
    struct Process processes[10], temp_process;
    float average_turnaround_time = 0, average_waiting_time = 0;

    srand(time(NULL)); // Seed the random number generator with the current time

    printf("<-- Highest Priority First Scheduling Algorithm (Preemptive) -->\n");
    printf("Enter Number of Processes: ");
    scanf("%d", &num_processes);

    for (i = 0; i < num_processes; i++)
    {
        processes[i] = generate_random_process(i + 1);
    }

    remaining_processes = num_processes;

    // Sort processes by arrival time
    for (i = 0; i < num_processes - 1; i++)
    {
        for (int j = 0; j < num_processes - i - 1; j++)
        {
            if (processes[j].arrival_time > processes[j + 1].arrival_time)
            {
                temp_process = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp_process;
            }
        }
    }

    max_priority = processes[0].temp_priority;
    max_priority_index = 0;

    // Find the process with the highest priority among those that have arrived
    for (int j = 0; j < num_processes && processes[j].arrival_time <= processes[0].arrival_time; j++)
    {
        if (processes[j].temp_priority > max_priority)
        {
            max_priority = processes[j].temp_priority;
            max_priority_index = j;
        }
    }

    i = max_priority_index;
    current_time = processes[i].completion_time = processes[i].arrival_time + 1;
    processes[i].remaining_time--;

    if (processes[i].remaining_time == 0)
    {
        processes[i].temp_priority = -9999; // Mark as completed
        remaining_processes--;
    }

    while (remaining_processes > 0)
    {
        max_priority = processes[0].temp_priority;
        max_priority_index = 0;

        // Find the process with the highest priority among those that have arrived
        for (int j = 0; j < num_processes && processes[j].arrival_time <= current_time; j++)
        {
            if (processes[j].temp_priority > max_priority)
            {
                max_priority = processes[j].temp_priority;
                max_priority_index = j;
            }
        }

        i = max_priority_index;
        processes[i].completion_time = current_time = current_time + 1;
        processes[i].remaining_time--;

        if (processes[i].remaining_time == 0)
        {
            processes[i].temp_priority = -9999; // Mark as completed
            remaining_processes--;
        }
    }

    printf("\nProcess No\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");

    for (i = 0; i < num_processes; i++)
    {
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        average_turnaround_time += processes[i].turnaround_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        average_waiting_time += processes[i].waiting_time;

        printf("Process %d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n",
               processes[i].process_no,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
    }

    average_turnaround_time /= num_processes;
    average_waiting_time /= num_processes;

    printf("\nAverage Turnaround Time = %f\nAverage Waiting Time = %f\n", average_turnaround_time, average_waiting_time);


    return 0;
}

/******************************************************************************
Results of the simulaton with the ideal scenario of a perfect scheduler

Metric	     Priority cheduling Algorithm (Preemptive)	Perfect Scheduler
Throughput	 	 	 	 Good	 	 	 	 	 	 	 	Excellent
Response time	 	     Good		 	 	 	 	 	 	Excellent
Turnaround time		 	 Good	 	 	 	 	 	 	 	Excellent
Fairness		 	 	 Fair	 	 	 	 	 	 	 	Excellent


Report on the Comparative Analysis of Priority Scheduling, Shortest Job First (SJF), and First Come First Serve (FCFS) Scheduling Algorithms

Process Arrival Time Priority Burst Time
P1         0             3         8
P2         0             4         7
P3         0             1         8
P4         0             2         5
P5         0             5         6

Analysis:

Priority Scheduling:
Priority scheduling is a non-preemptive scheduling algorithm that assigns priorities to processes, with higher values indicating higher priority. The waiting time and turnaround time for each process under the Priority Scheduling algorithm are as follows:
Process Waiting Time Turnaround Time
P1         11               19
P2         7                14
P3         0                8
P4         8               13
P5        14               20
Average Waiting Time: (11 + 7 + 0 + 8 + 14) / 5 = 8
Average Turnaround Time: (19 + 14 + 8 + 13 + 20) / 5 = 14.8

Shortest Job First (SJF):
SJF is a preemptive scheduling algorithm that selects the process with the shortest burst time. The waiting time and turnaround time for each process under SJF are as follows:
Process Waiting Time Turnaround Time
P1          9             17
P2          0             7
P3          16            24
P4          7             12
P5          12            18
Average Waiting Time: (9 + 0 + 16 + 7 + 12) / 5 = 8.8
Average Turnaround Time: (17 + 7 + 24 + 12 + 18) / 5 = 15.6

First Come First Serve (FCFS):
FCFS is a non-preemptive scheduling algorithm where processes are executed in the order they arrive. The waiting time and turnaround time for each process under FCFS are as follows:
Process Waiting Time Turnaround Time
P1           0            8
P2           8            15
P3           15           23
P4           23           28
P5           28           34
Average Waiting Time: (0 + 8 + 15 + 23 + 28) / 5 = 14.8
Average Turnaround Time: (8 + 15 + 23 + 28 + 34) / 5 = 21.6

Findings:

Average Waiting Time:
Priority Scheduling: The Priority Scheduling algorithm results in the lowest average waiting time in this scenario. It prioritizes processes based on their assigned priority.
SJF: SJF follows closely, with a slightly higher average waiting time, as it gives priority to shorter jobs.
FCFS: FCFS has the highest average waiting time, as it serves processes strictly in the order of arrival, potentially leading to longer waiting times for some processes.

Average Turnaround Time:
Priority Scheduling: Priority Scheduling also yields the lowest average turnaround time. It ensures that high-priority processes complete quickly.
SJF: SJF exhibits a moderate average turnaround time, as it prioritizes shorter jobs but may not be optimal for all scenarios.
FCFS: FCFS results in the highest average turnaround time, especially for longer jobs that arrive early in the queue.

Conclusion:

In this comparative analysis of Priority Scheduling, Shortest Job First (SJF), and First Come First Serve (FCFS) scheduling algorithms, the following conclusions can be drawn:

Priority Scheduling is an effective choice when prioritizing processes based on their importance or criticality is essential. It minimizes waiting and turnaround times for high-priority processes.
SJF is a good option when minimizing waiting and turnaround times is a priority, and the burst times of processes are known in advance. However, it may not be suitable for all scenarios.
FCFS is a straightforward but less efficient algorithm in terms of minimizing waiting and turnaround times. It may lead to longer waiting times, especially for processes with longer burst times, making it less suitable for real-time systems or scenarios where fairness and efficiency are critical.

*******************************************************************************/
