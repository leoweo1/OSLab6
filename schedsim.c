// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    // 1. Create an array rem_bt[] to keep track of remaining burst time of processes
    int rem_bt[n];
    for (int i = 0; i < n; i++) 
        rem_bt[i] = plist[i].bt;
    
    // 2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
    for (int i = 0; i < n; i++) 
        plist[i].wt = 0;
    
    // 3. Initialize time
    int t = 0;
    
    // 4. Keep traversing while all processes are not done
    while (1) {
        bool done = true;
        
        for (int i = 0; i < n; i++) {
            // Check if process has arrived and not completed
            if (plist[i].art <= t && rem_bt[i] > 0) {
                done = false; // There is a pending process
                
                if (rem_bt[i] > quantum) {
                    // If remaining burst time > quantum
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    // Last cycle for this process
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt - plist[i].art;
                    rem_bt[i] = 0; // This process is over
                }
            }
        }
        
        // If all processes are done
        if (done) {
            // Advance time if no process has arrived yet
            bool all_arrived = false;
            for (int i = 0; i < n; i++) {
                if (plist[i].art > t) {
                    all_arrived = false;
                    break;
                }
                all_arrived = true;
            }
            if (!all_arrived) t++;
            else break;
        }
    }
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
    int rem_bt[n];
    int completed = 0;
    int current_time = 0;
    int shortest = -1;
    int min_rem_time = INT_MAX;
    bool check = false;
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }
    
    while (completed != n) {
        // Find process with minimum remaining time among arrived processes
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= current_time && rem_bt[i] > 0) {
                if (rem_bt[i] < min_rem_time) {
                    min_rem_time = rem_bt[i];
                    shortest = i;
                    check = true;
                }
            }
        }
        
        if (!check) {
            current_time++;
            continue;
        }
        
        // Reduce remaining time by 1
        rem_bt[shortest]--;
        min_rem_time = rem_bt[shortest];
        
        // If min_rem_time becomes 0, reset it
        if (min_rem_time == 0) {
            min_rem_time = INT_MAX;
        }
        
        // If process is completed
        if (rem_bt[shortest] == 0) {
            completed++;
            check = false;
            
            // Calculate waiting time
            int finish_time = current_time + 1;
            plist[shortest].wt = finish_time - plist[shortest].bt - plist[shortest].art;
            
            // If waiting time is negative, set to 0
            if (plist[shortest].wt < 0) 
                plist[shortest].wt = 0;
        }
        
        current_time++;
    }
} 

// Function to find the waiting time for FCFS  
void findWaitingTime(ProcessType plist[], int n)
{ 
    // Sort by arrival time first to ensure correct order
    // Calculate waiting time
    plist[0].wt = 0;
    
    for (int i = 1; i < n; i++) {
        // Completion time of previous process
        int prev_completion = plist[i-1].art + plist[i-1].wt + plist[i-1].bt;
        
        // If current process arrives after previous completes
        if (plist[i].art >= prev_completion) {
            plist[i].wt = 0;
        } else {
            plist[i].wt = prev_completion - plist[i].art;
        }
    }
} 
  
// Function to calculate turn around time 
void findTurnAroundTime(ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int i = 0; i < n; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
    // Cast to ProcessType pointers
    ProcessType *p1 = (ProcessType *)this;
    ProcessType *p2 = (ProcessType *)that;
    
    // CORRECTED: Higher priority has LOWER number (0 is highest)
    // So we sort in ASCENDING order of priority (0 comes first)
    if (p1->pri < p2->pri) return -1;  // p1 has higher priority
    if (p1->pri > p2->pri) return 1;   // p2 has higher priority
    
    // If same priority, sort by arrival time (FCFS for same priority)
    if (p1->art < p2->art) return -1;
    if (p1->art > p2->art) return 1;
    
    // If same arrival time, sort by process ID
    if (p1->pid < p2->pid) return -1;
    if (p1->pid > p2->pid) return 1;
    
    return 0;
} 

// Function to sort by arrival time
int compare_arrival_time(const void *a, const void *b) {
    ProcessType *p1 = (ProcessType *)a;
    ProcessType *p2 = (ProcessType *)b;
    
    if (p1->art < p2->art) return -1;
    if (p1->art > p2->art) return 1;
    
    // If same arrival time, sort by process ID
    if (p1->pid < p2->pid) return -1;
    if (p1->pid > p2->pid) return 1;
    
    return 0;
}

// Function to sort by burst time (for non-preemptive SJF if needed)
int compare_burst_time(const void *a, const void *b) {
    ProcessType *p1 = (ProcessType *)a;
    ProcessType *p2 = (ProcessType *)b;
    
    if (p1->bt < p2->bt) return -1;
    if (p1->bt > p2->bt) return 1;
    
    // If same burst time, sort by arrival time
    if (p1->art < p2->art) return -1;
    if (p1->art > p2->art) return 1;
    
    return 0;
}

//Function to calculate average time 
void findavgTimeFCFS(ProcessType plist[], int n) 
{ 
    // Create a copy to avoid modifying original order for other algorithms
    ProcessType *sorted_list = malloc(n * sizeof(ProcessType));
    memcpy(sorted_list, plist, n * sizeof(ProcessType));
    
    // Sort processes by arrival time
    qsort(sorted_list, n, sizeof(ProcessType), compare_arrival_time);
    
    // Function to find waiting time of all processes 
    findWaitingTime(sorted_list, n); 
  
    // Function to find turn around time for all processes 
    findTurnAroundTime(sorted_list, n); 
  
    // Copy results back to original array (preserving original order)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (plist[i].pid == sorted_list[j].pid) {
                plist[i].wt = sorted_list[j].wt;
                plist[i].tat = sorted_list[j].tat;
                break;
            }
        }
    }
    
    free(sorted_list);
    
    // Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF(ProcessType plist[], int n) 
{ 
    // Create a copy to avoid modifying original order
    ProcessType *sorted_list = malloc(n * sizeof(ProcessType));
    memcpy(sorted_list, plist, n * sizeof(ProcessType));
    
    // Sort by arrival time first (SJF is preemptive)
    qsort(sorted_list, n, sizeof(ProcessType), compare_arrival_time);
    
    // Function to find waiting time of all processes 
    findWaitingTimeSJF(sorted_list, n); 
  
    // Function to find turn around time for all processes 
    findTurnAroundTime(sorted_list, n); 
  
    // Copy results back to original array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (plist[i].pid == sorted_list[j].pid) {
                plist[i].wt = sorted_list[j].wt;
                plist[i].tat = sorted_list[j].tat;
                break;
            }
        }
    }
    
    free(sorted_list);
    
    // Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    // Create a copy to avoid modifying original order
    ProcessType *sorted_list = malloc(n * sizeof(ProcessType));
    memcpy(sorted_list, plist, n * sizeof(ProcessType));
    
    // Sort by arrival time first
    qsort(sorted_list, n, sizeof(ProcessType), compare_arrival_time);
    
    // Function to find waiting time of all processes 
    findWaitingTimeRR(sorted_list, n, quantum); 
  
    // Function to find turn around time for all processes 
    findTurnAroundTime(sorted_list, n); 
  
    // Copy results back to original array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (plist[i].pid == sorted_list[j].pid) {
                plist[i].wt = sorted_list[j].wt;
                plist[i].tat = sorted_list[j].tat;
                break;
            }
        }
    }
    
    free(sorted_list);
    
    // Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority(ProcessType plist[], int n) 
{ 
    // Create a copy to avoid modifying original order
    ProcessType *sorted_list = malloc(n * sizeof(ProcessType));
    memcpy(sorted_list, plist, n * sizeof(ProcessType));
    
    // 1- Sort the processes according to priority (0 = highest priority)
    qsort(sorted_list, n, sizeof(ProcessType), my_comparer);
    
    // 2- Now apply FCFS algorithm to the sorted list
    findWaitingTime(sorted_list, n);
    findTurnAroundTime(sorted_list, n);
  
    // Copy results back to original array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (plist[i].pid == sorted_list[j].pid) {
                plist[i].wt = sorted_list[j].wt;
                plist[i].tat = sorted_list[j].tat;
                break;
            }
        }
    }
    
    free(sorted_list);
    
    // Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn around time 
    for (int i = 0; i < n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;  // FIXED: Was returning "pllist" which doesn't exist
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
    // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeFCFS(proc_list, n);
    printMetrics(proc_list, n);
    free(proc_list);
  
    // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeSJF(proc_list, n); 
    printMetrics(proc_list, n);
    free(proc_list);
  
    // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    findavgTimePriority(proc_list, n); 
    printMetrics(proc_list, n);
    free(proc_list);
    
    // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeRR(proc_list, n, quantum); 
    printMetrics(proc_list, n);
    free(proc_list);
    
    return 0; 
}
