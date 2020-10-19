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

	for(int i = 0; i < n; i++) {
		processes[i].waitingTime = currentTime - processes[i].arrivalTime;
		processes[i].turnAroundTime = processes[i].cpuBurst + processes[i].waitingTime;
		
		currentTime += processes[i].cpuBurst;
		totalWaitingTime += processes[i].waitingTime;
		totalTurnAroundTime += processes[i].turnAroundTime;

	}
	
	printf("Process\tBurst\tArrival\tWaiting\tTurn Around\n");
	for(int i = 0; i < n ; i++) {
		printf("P%d\t%d\t%d\t%d\t%d\n", processes[i].processId, processes[i].cpuBurst, processes[i].arrivalTime, processes[i].waitingTime, processes[i].turnAroundTime);
	}
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
