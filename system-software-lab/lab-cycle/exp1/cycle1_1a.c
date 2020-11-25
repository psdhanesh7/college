#include<stdio.h>
#include<stdlib.h>

typedef struct {
	
	int processId;
	int arrivalTime;
	int cpuBurst;
	int waitingTime;
	int turnAroundTime;

} process;

void scheduleFCFS(process *processes, int n) {
		
	int currentTime = 0;
	int totalWaitingTime = 0;
	int totalTurnAroundTime = 0;

	printf("---------------------\n");	
	printf("Gantt Chart for FCFS\n");
	printf("---------------------\n");

	for(int i = 0; i < n; i++) {
		printf("P%d: %d - ", i, currentTime);
		
		processes[i].waitingTime = currentTime - processes[i].arrivalTime;
		processes[i].turnAroundTime = processes[i].cpuBurst + processes[i].waitingTime;
		
		currentTime += processes[i].cpuBurst;
		totalWaitingTime += processes[i].waitingTime;
		totalTurnAroundTime += processes[i].turnAroundTime;
		
		printf("%d\n", currentTime);
	}
	printf("----------------------\n");


	float averageWaitingTime = (float) totalWaitingTime / n;
	float averageTurnAroundTime = (float) totalTurnAroundTime / n;

	printf("Average waiting time : %f\n", averageWaitingTime);
	printf("Average turn around time : %f\n", averageTurnAroundTime);

}

void main() {
	
	int n;

	printf("Enter the number of process: ");
	scanf("%d", &n);

	process* processes = (process*) malloc(n * sizeof(process));

	for(int i = 0; i < n; i++) {
		printf("Enter the arrival time and CPU time for process P%d : ", i);
		scanf("%d %d", &(processes[i].arrivalTime), &(processes[i].cpuBurst));
		processes[i].processId = i+1;
	}
	
	scheduleFCFS(processes, n);
	
	free(processes);	
}

// Input

// 5
// 0 8
// 1 4
// 2 1
// 3 5
// 4 2
