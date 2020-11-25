
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

// Defining structure for process
typedef struct {
	
	int processId;	
	int arrivalTime;
	int cpuBurst;
	int completed;
	int waitingTime;
	int turnAroundTime;

} process;

process* findShortestJob(process* processes, int n, int currentTime) {
	
	int i = 0;
	process* min;
	int minTime = INT_MAX;	
	while(processes[i].arrivalTime <= currentTime && i < n) {
		if(!processes[i].completed && processes[i].cpuBurst < minTime) {
			min = &processes[i];
			minTime = processes[i].cpuBurst;
		}
		i++;
	}

	return min;
}

// Scheduling algorithm
void scheduleSJF(process *processes, int n) {
		
	int currentTime = 0;
	int completed = 0;
	int totalWaitingTime = 0;
	int totalTurnAroundTime = 0;
	
	printf("-------------------\n");
	printf("Gantt chart for SJF\n");
	printf("-------------------\n");
		
	while(completed < n) {
		
		process* currentProcess = findShortestJob(processes, n, currentTime);
		
		printf("P%d : %d - %d\n", currentProcess->processId, currentTime, currentTime + currentProcess->cpuBurst);

		currentProcess->waitingTime = currentTime - currentProcess->arrivalTime;

		totalWaitingTime += currentProcess->waitingTime;

		currentTime += currentProcess->cpuBurst;
		currentProcess->completed = 1;
		currentProcess->turnAroundTime = currentTime - currentProcess->arrivalTime;
		totalTurnAroundTime += currentProcess->turnAroundTime;
		completed++;
	}

	printf("------------------\n");

	float averageWaitingTime = (float) totalWaitingTime / n;
	float averageTurnAroundTime = (float) totalTurnAroundTime / n;

	printf("Average waiting time : %f\n", averageWaitingTime);
	printf("Average turn around time : %f\n", averageTurnAroundTime);
}

void main() {

	int n;
	
	printf("Enter the number of process: ");
	scanf("%d", &n);

	process *processes = (process *) malloc(sizeof(process));
	
	for(int i = 0; i < n; i++) {
		printf("Enter the arrival time and CPU time for process P%d : ", i);
		scanf("%d %d", &(processes[i].arrivalTime), &(processes[i].cpuBurst));
		processes[i].processId = i;
		processes[i].completed = 0;
	}
	
	scheduleSJF(processes, n);

	free(processes);
}

// Input

// 5
// 0 8
// 1 4
// 2 1
// 3 5
// 4 2