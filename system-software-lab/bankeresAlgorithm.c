#include<stdio.h>

#define resources 3

int getNextPossibleProcess(int required[][resources], int available[], int completed[], int n) {

	int process = -1;

	for(int i = 0; i < n; i++ ) {
		if(!completed[i]) {
			int allowable = 1;

			for(int j = 0; j < resources; j++) {
				if(required[i][j] > available[j]) {
					allowable = 0;
					break;
				}
			}

			if(allowable) {
				process = i;
				return process;
			}
		}
	}

	return process;
}

void main() {

	int n; // Number of processes
	int allocation[10][resources], max[10][resources], required[10][resources];
	int totalResource[] = {10, 5, 7};
	int available[resources];
	int completed[10];
	int safeSequence[10];

	for(int i = 0; i < resources; i++) {
		available[i] = totalResource[i];
	}

	printf("Enter the number of process : ");
	scanf("%d", &n);

	printf("Enter the maximum required resources matrix :\n");
	for(int i = 0; i < n; i++) {
		
		for(int j = 0; j < resources; j++) {
			scanf("%d", &max[i][j]);
		}

		completed[i] = 0; // setting each process as not completed
	}

	printf("Enter the current allocation matrix : \n");
	for(int i = 0; i <  n; i++) {
		
		for(int j = 0; j < resources; j++) {
			scanf("%d", &allocation[i][j]);

			available[j] -= allocation[i][j]; // calculating the currently available instances of each resource

			required[i][j] = max[i][j] - allocation[i][j]; // calcaulting the required number of instances for each process
		}
	}

	int completedProcess = 0; // count of the completed process
	while(completedProcess < n) {

		int nextProcess = getNextPossibleProcess(required, available, completed, n);

		if(nextProcess == -1) {
			break;
		}

		safeSequence[completedProcess] = nextProcess;

		completed[nextProcess] = 1;
		completedProcess++;

		// releasing the resources of the completed process
		for(int i = 0; i < resources; i++) {
			available[i] += allocation[nextProcess][i];
		}
	}

	if(completedProcess != n) {
		printf("System went into an usafe state!!!\n");
	}
	else {
		printf("System is in a safe state\n");
		printf("Safe sequence is the following : \n");
		for(int i = 0; i < n; i++) {
			printf("P%d\t", safeSequence[i]);
		}
		printf("\n");
	}
}

