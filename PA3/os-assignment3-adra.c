#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* 
This is a CPU scheduling sim
*/

#define MAX_PROCESSES 100

/* Structure to represent a process */
typedef struct {
    char pid[10];
    int at, bt, pr, wt, tat, rt, remaining_bt;
    int completion_time;
    int first_response_time;
} Process;

/*
at: arrival time : The time at which the process arrives in the ready queue
bt: burst time : The total time required by the process for execution on the CPU
pr: priority : The priority of the process (lower value indicates higher priority)
wt: waiting time : The total time the process has been in the ready queue (turnaround time - burst time)
tat: turnaround time : The total time taken from arrival to completion of the process
rt: response time : The time from arrival to the first time the process gets the CPU
remaining_bt: remaining burst time : The remaining time required by the process for execution
completion_time: The time at which the process completes its execution
first_response_time: The time at which the process first gets the CPU
*/

/* struct for gantt */
typedef struct {
    char segment[10];
    int start, end;
} GanttSegment;

/* read from stdin for # processes : pass by reference */
void readProcessStdin(Process processes[], int *n) {
    printf("Enter number of processes: ");
    scanf("%d", n);
    for (int i = 0; i < *n; i++) {
        scanf("%s %d %d %d", processes[i].pid, &processes[i].at, &processes[i].bt, &processes[i].pr);
        // initialize process fields
        processes[i].remaining_bt = processes[i].bt;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].rt = -1;  // Initialize to -1 for not responded yet
        processes[i].completion_time = 0;
        processes[i].first_response_time = -1;
    }
}

int compareArrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->at - p2->at;
}

/* sort by arrival time */
void sortByArrivalTime(Process processes[], int n) {
    qsort(processes, n, sizeof(Process), compareArrival);
}

/* first come first serve */
void firstComeFirstServe(Process processes[], int n, GanttSegment gantt[], int *gantt_count) {
    sortByArrivalTime(processes, n);
    int current_time = 0;
    *gantt_count = 0;

    // Loop through all processes
    for (int i = 0; i < n; i++) {
        // Handle case current time < arrival time
        if (current_time < processes[i].at) {
            // CPU is idle
            current_time = processes[i].at;
        }
        processes[i].first_response_time = current_time;
        processes[i].rt = current_time - processes[i].at;
        processes[i].completion_time = current_time + processes[i].bt;
        processes[i].tat = processes[i].completion_time - processes[i].at;
        processes[i].wt = processes[i].tat - processes[i].bt;

        strcpy(gantt[*gantt_count].segment, processes[i].pid);
        gantt[*gantt_count].start = current_time;
        gantt[*gantt_count].end = current_time + processes[i].bt;
        (*gantt_count)++;

        current_time = processes[i].completion_time;
    }
}

/* shortest job first non-preemptive */
void shortestJobFirst(Process processes[], int n, GanttSegment gantt[], int *gantt_count) {
    sortByArrivalTime(processes, n);
    int current_time = 0;
    int completed = 0;
    *gantt_count = 0;

    while (completed < n) {
        int selected = -1;
        int min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= current_time &&
                processes[i].remaining_bt > 0 &&
                processes[i].remaining_bt < min_bt) {
                min_bt = processes[i].remaining_bt;
                selected = i;
            }
        }
        if (selected == -1) {
            current_time++;
            continue;
        }
        if (processes[selected].first_response_time == -1) {
            processes[selected].first_response_time = current_time;
            processes[selected].rt = current_time - processes[selected].at;
        }
        int exec_time = processes[selected].remaining_bt;
        int start_time = current_time;
        current_time += exec_time;
        processes[selected].remaining_bt = 0;
        processes[selected].completion_time = current_time;
        processes[selected].tat = current_time - processes[selected].at;
        processes[selected].wt = processes[selected].tat - processes[selected].bt;
        completed++;

        strcpy(gantt[*gantt_count].segment, processes[selected].pid);
        gantt[*gantt_count].start = start_time;
        gantt[*gantt_count].end = current_time;
        (*gantt_count)++;
    }
}

/* priority scheduling preemptive */
void prioritySchedulingPreemptive(Process processes[], int n, GanttSegment gantt[], int *gantt_count) {
    sortByArrivalTime(processes, n);
    int current_time = 0;
    int completed = 0;
    *gantt_count = 0;
    char current_pid[10] = {0};

    while (completed < n) {
        int selected = -1;
        int highest_priority = INT_MAX;  // Lower pr value is higher priority
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= current_time &&
                processes[i].remaining_bt > 0 &&
                processes[i].pr < highest_priority) {
                highest_priority = processes[i].pr;
                selected = i;
            }
        }
        if (selected == -1) {
            current_time++;
            continue;
        }
        if (processes[selected].first_response_time == -1) {
            processes[selected].first_response_time = current_time;
            processes[selected].rt = current_time - processes[selected].at;
        }
        // Check if new segment or continue
        int new_segment = 1;
        if (*gantt_count > 0 && strcmp(current_pid, processes[selected].pid) == 0) {
            new_segment = 0;
        }
        if (new_segment) {
            strcpy(gantt[*gantt_count].segment, processes[selected].pid);
            gantt[*gantt_count].start = current_time;
            strcpy(current_pid, processes[selected].pid);
            (*gantt_count)++;
        }
        // Execute one unit
        processes[selected].remaining_bt--;
        current_time++;
        gantt[*gantt_count - 1].end = current_time;
        if (processes[selected].remaining_bt == 0) {
            processes[selected].completion_time = current_time;
            processes[selected].tat = current_time - processes[selected].at;
            processes[selected].wt = processes[selected].tat - processes[selected].bt;
            completed++;
            current_pid[0] = '\0';
        }
    }
}

/* round robin */
void roundRobin(Process processes[], int n, int quantum, GanttSegment gantt[], int *gantt_count) {
    sortByArrivalTime(processes, n);
    int current_time = 0;
    int completed = 0;
    *gantt_count = 0;
    int queue[MAX_PROCESSES];
    int front = 0;
    int rear = 0;
    int next_arrival_idx = 0;

    while (completed < n) {
        // Enqueue arrived processes
        while (next_arrival_idx < n && processes[next_arrival_idx].at <= current_time) {
            queue[rear++] = next_arrival_idx;
            next_arrival_idx++;
        }
        if (front == rear) {
            // No process ready, advance to next arrival
            if (next_arrival_idx < n) {
                current_time = processes[next_arrival_idx].at;
                continue;
            } else {
                break;
            }
        }
        int selected = queue[front++];
        if (processes[selected].first_response_time == -1) {
            processes[selected].first_response_time = current_time;
            processes[selected].rt = current_time - processes[selected].at;
        }
        int exec_time = (processes[selected].remaining_bt < quantum) ? processes[selected].remaining_bt : quantum;
        int start_time = current_time;
        current_time += exec_time;
        processes[selected].remaining_bt -= exec_time;

        strcpy(gantt[*gantt_count].segment, processes[selected].pid);
        gantt[*gantt_count].start = start_time;
        gantt[*gantt_count].end = current_time;
        (*gantt_count)++;

        // Enqueue more arrived during execution
        while (next_arrival_idx < n && processes[next_arrival_idx].at <= current_time) {
            queue[rear++] = next_arrival_idx;
            next_arrival_idx++;
        }
        // Re-enqueue if not completed
        if (processes[selected].remaining_bt > 0) {
            queue[rear++] = selected;
        } else {
            processes[selected].completion_time = current_time;
            processes[selected].tat = current_time - processes[selected].at;
            processes[selected].wt = processes[selected].tat - processes[selected].bt;
            completed++;
        }
    }
}

/* print gantt chart */

/*
NOTE: out defaults to stdout for screen output. 
Only overrides to file if chosen. Both cases covered.
*/

void printGanttChart(GanttSegment gantt[], int gantt_count, FILE *out) {
    fprintf(out, "[Gantt Chart]\n|");
    for (int i = 0; i < gantt_count; i++) {
        fprintf(out, " %s |", gantt[i].segment);
    }
    fprintf(out, "\n");
}

/* print process table */
void printProcessTable(Process processes[], int n, FILE *out) {
    fprintf(out, "[Table]\nPID | AT | BT | PR | WT | TAT | RT\n");
    for (int i = 0; i < n; i++) {
        fprintf(out, "%s: |  %d |  %d | %d  | %d  | %d   | %d\n", processes[i].pid, processes[i].at, processes[i].bt, processes[i].pr,
               processes[i].wt, processes[i].tat, processes[i].rt);
    }
}

/* print averages */
void printAverages(Process processes[], int n, FILE *out) {
    double sum_wt = 0, sum_tat = 0, sum_rt = 0;
    for (int i = 0; i < n; i++) {
        sum_wt += processes[i].wt;
        sum_tat += processes[i].tat;
        sum_rt += processes[i].rt;
    }
    fprintf(out, "Average WT = %.2f\nAverage TAT = %.2f\nAverage RT = %.2f\n", sum_wt / n, sum_tat / n, sum_rt / n);
}

void readProcessFile(Process processes[], int *n, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "File error\n");
        exit(1);
    }
    fscanf(fp, "%d", n);
    for (int i = 0; i < *n; i++) {
        fscanf(fp, "%s %d %d %d", processes[i].pid, &processes[i].at, &processes[i].bt, &processes[i].pr);
        // initialize process fields
        processes[i].remaining_bt = processes[i].bt;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].rt = -1;  // Initialize to -1 for not responded yet
        processes[i].completion_time = 0;
        processes[i].first_response_time = -1;
    }
    fclose(fp);

}

int main() {
    Process processes[MAX_PROCESSES];
    GanttSegment gantt[1000];  // Sufficient space for preemptive segments
    int n;
    int algo_choice, quantum = 0;

    printf("==== CPU Scheduling Simulator ====\n");
    // Ask for input choice
    int input_choice;
    printf("Input:\n1. Manual\n2. File\nChoice: ");
    scanf("%d", &input_choice);

    if (input_choice == 2) {
        char filename[100];
        printf("Enter file name: ");
        scanf("%s", filename);
        readProcessFile(processes, &n, filename);
    } else {
        readProcessStdin(processes, &n);
    }


    printf("Choose algorithm:\n1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\nAlgorithm: ");
    scanf("%d", &algo_choice);
    if (algo_choice == 4) {
        printf("Quantum: ");
        scanf("%d", &quantum);
    }

    int gantt_count = 0;
    switch (algo_choice) {
        case 1:
            firstComeFirstServe(processes, n, gantt, &gantt_count);
            break;
        case 2:
            shortestJobFirst(processes, n, gantt, &gantt_count);
            break;
        case 3:
            prioritySchedulingPreemptive(processes, n, gantt, &gantt_count);
            break;
        case 4:
            roundRobin(processes, n, quantum, gantt, &gantt_count);
            break;
        default:
            printf("Invalid algorithm choice\n");
            return 1;
    }

    int output_choice;
    printf("Output:\n1. File\n2. Screen\nChoice: ");
    scanf("%d", &output_choice);

    FILE *out = stdout; // initialize out ptr to stdout

    char out_filename[100] = {0};
    if (output_choice == 1) {
        printf("Enter file name: ");
        scanf("%s", out_filename);

        // override out ptr to file
        out = fopen(out_filename, "w");
        if (!out) { printf("File error\n"); return 1; }
    }

    printGanttChart(gantt, gantt_count, out);
    printProcessTable(processes, n, out);
    printAverages(processes, n, out);
    
    if (output_choice == 1) {
        fclose(out);
        printf("%s successfully created\n", out_filename);
    }
    return 0;
}