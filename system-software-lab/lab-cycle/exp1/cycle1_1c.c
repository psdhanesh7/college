#include<stdio.h>
#include<stdlib.h>

#define MAX 100

typedef struct {

	int processId;
	int arrivalTime;
	int cpuBurst;
	int remainingTime;
	int lastSlice;
	int waitingTime;
	int turnAroundTime;

} process;

process* q[MAX];
int front = -1, rear = -1;

void enque(process* value) {
	if(rear == MAX-1) {
		return;
	}
	else if(front == -1) {
		front = rear = 0;
		q[rear] = value;
	}
	else {
		rear++;
		q[rear] = value;
	}
}

process* deque() {
	process* ans;
	if(front == -1) {
		return ans;
	}
	else if(front == rear) {
		ans = q[front];
		front = rear = -1;
	}
	else {
		ans = q[front];
		front++;
	}	

	return ans;
}

int isEmpty() {
	return front == -1 ? 1 : 0;
}


void scheduleRR(process* processes, int n, int quantum) {
	
	int currentTime = 0;
	int totalWaitingTime = 0;
	int totalTurnAroundTime = 0;
	int lastIndexInQueue = 1;
	
	printf("---------------------------\n");
	printf("Gantt chart for Round Robin\n");
	printf("---------------------------\n");

	enque(&processes[0]);
	while(!isEmpty()) {
		
		process *currentProcess = deque();
		
		printf("P%d: %d - ", currentProcess->processId, currentTime);

		int waitingTime = currentTime - currentProcess->lastSlice;
		totalWaitingTime += waitingTime;
		currentProcess->waitingTime += waitingTime;

		if(currentProcess->remainingTime <= quantum) {
			currentTime += currentProcess->remainingTime;
			currentProcess->remainingTime = 0;

			currentProcess->turnAroundTime = currentTime - currentProcess->arrivalTime;
			totalTurnAroundTime += currentProcess->turnAroundTime;
		}
		else {
			currentTime += quantum;
			currentProcess->remainingTime -= quantum;
		}
		
		printf("%d\n", currentTime);

		currentProcess->lastSlice = currentTime;
		
		int i = lastIndexInQueue ;
		while(i < n && processes[i].arrivalTime <= currentTime) {
			if(i == currentProcess->processId) {
				i++;
				continue;
			}
			if(processes[i].remainingTime > 0) {
				enque(&processes[i]);
			}
			i++;
		}
		lastIndexInQueue = i;
		
		if(currentProcess->remainingTime > 0) {
			 enque(currentProcess);
		}
		
	}

	printf("---------------------------\n");
	
	float averageTurnAround = (float) totalTurnAroundTime / n;
	float averageWaitingTime = (float) totalWaitingTime / n;

	printf("Average turn around time: %f\n", averageTurnAround);
	printf("Average waiting time: %f\n", averageWaitingTime);

}

void main() {
	
	int n, quantum;
	
	printf("Enter the CPU time quantum: ");
	scanf("%d", &quantum);

	printf("Enter the number of process: ");
	scanf("%d", &n);

	process* processes = (process*) malloc(n * sizeof(process));

	for(int i = 0; i < n; i++) {
		printf("Enter the arrival time and CPU time for process P%d : ", i);
		scanf("%d %d", &(processes[i].arrivalTime), &(processes[i].cpuBurst));
		
		processes[i].processId = i;
		processes[i].remainingTime = processes[i].cpuBurst;
		processes[i].lastSlice = processes[i].arrivalTime;
	}

	scheduleRR(processes, n, quantum);
	
	free(processes);	
}

// Input

// 4
// 5
// 0 11
// 5 13
// 9 6
// 13 9
// 17 12