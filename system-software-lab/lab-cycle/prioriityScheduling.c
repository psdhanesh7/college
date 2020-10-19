#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX 100

// Defining structure for process
typedef struct {
	
	int processId;	
	int arrivalTime;
	int cpuBurst;
	int priority;
	int completed;
	int waitingTime;
	int turnAroundTime;

} process;

process* findHighestPriorityJob(process* processes, int n, int currentTime) {
	
	int i = 0;
	process* max;
	int maxPriority = INT_MIN;	
	while(processes[i].arrivalTime <= currentTime && i < n) {
		if(!processes[i].completed && processes[i].priority > maxPriority) {
			max = &processes[i];
			maxPriority = processes[i].priority;
		}
		i++;
	}

	return max;
}

// Scheduling algorithm
void schedulePriority(process *processes, int n) {
		
	int currentTime = 0;
	int completed = 0;
	int totalWaitingTime = 0;
	int totalTurnAroundTime = 0;
	
	while(completed < n) {
		
		process* currentProcess = findHighestPriorityJob(processes, n, currentTime);
		
		printf("P%d : %d - %d\n", currentProcess->processId, currentTime, currentTime + currentProcess->cpuBurst);

		currentProcess->waitingTime = currentTime - currentProcess->arrivalTime;

		totalWaitingTime += currentProcess->waitingTime;

		currentTime += currentProcess->cpuBurst;
		currentProcess->completed = 1;
		currentProcess->turnAroundTime = currentTime - currentProcess->arrivalTime;

		totalTurnAroundTime += currentProcess->turnAroundTime;
		completed++;
	}

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
		printf("Enter the arrival time, CPU time and priority for process P%d : ", i);
		scanf("%d %d %d", &(processes[i].arrivalTime), &(processes[i].cpuBurst), &(processes[i].priority));
		processes[i].processId = i;
		processes[i].completed = 0;
	}
	
	schedulePriority(processes, n);

	free(processes);
}

