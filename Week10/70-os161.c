#include <stdio.h>
#include <math.h>

typedef struct {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // CPU Burst Time
    int remaining_time; // For SRT/RR (starts equal to burst_time)
    int completion_time;// Time process finishes
    int waiting_time;   // Metrics
    int turnaround_time;// Metrics
    int is_completed;   // Flag for SJF/SRT
} Process;

void calculate_FCFS(Process processes[], int n) {
   int curr_time = 0;

   for (int i = 0; i < n; i++) {
       if (curr_time < processes[i].arrival_time) {
           curr_time = processes[i].arrival_time;
       }
       
       // execute process 
       curr_time += process[i].burst_time;

       // record metrics
       process[i].completion_time = curr_time;
       process[i].turnaround_time = process[i].completion_time - process[i].arrival_time;
       process[i].waiting_time = process[i].turnaround_time - process[i].burst_time;
   }
}

void calculate_SJF(Process processes[], int n) {
   int curr_time = 0;
   int completed = 0;

   while (completed != n) {
      int idx = -1;  
      int min_burst = HUGE_VAL;

      // selection logic
      for (int i = 0; i < n; i++) {
         if ()
      }
   }
}

int main(void) {
   int choice;
   printf("enter which algorithm to simulate: \n");
   printf("1. FCFS\n2. SJF\n3. SRT\n4. RR\n");

   scanf("%d", &choice);

   // Sample processes (assuming sorted by arrival time for FCFS)
   Process processes[] = {
       {1, 0, 5},
       {2, 1, 3},
       {3, 2, 8}
   };
   int n = 3;

   switch (choice) {
       case 1:
           calculate_FCFS(processes, n);
           // Print results
           printf("PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
           for (int i = 0; i < n; i++) {
               printf("%d\t%d\t%d\t%d\t\t%d\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time, processes[i].waiting_time, processes[i].turnaround_time);
           }
           break;
       default:
           printf("Invalid choice\n");
           break;
   }

   return 0;
}