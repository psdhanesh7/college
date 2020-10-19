
// Author : Dhanesh P S
// Date : 09/09/2020

// Problem Statement : Implement page replacement algorithms - FIFO

#include<stdio.h>

int memorySize;
int pages;

int q[100], size = 200, front = -1, rear = -1;

void enque(int value) {

	if(rear == size-1) {
		printf("Queue Overflow\n");
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

int deque() {
    
	if(front == -1) {
        return -1;
    }
	else if(front == rear) {
        int value = q[front];
		front = rear = -1;
        return value;
	}
	else {
        int value = q[front];
        front++;
        return value;
    }	
}

int getFreeMemoryIndex(int memory[], int n) {

    for(int i = 0; i < n; i++) {
        if(memory[i] == -1) {
            return i;
        }
    }
    return -1;
}
 
void swapMemory(int memory[], int n, int old, int new) {

    for(int i = 0; i < n; i++) {
        if(memory[i] == old) {
            memory[i] = new;
            break;
        }
    }
}

void getMemoryStatusFIFO(int referenceString[], int n) {

    int pageTable[10];
    int memory[memorySize]; // No. of frames is assumed to be 3
    int pageFaults = 0;

    // Set the queue to be empty
    front = -1;
    rear = -1;

    for(int i = 0; i < 8; i++) { // No. of pages is assumed to be 8
        pageTable[i] = 0; // Initailly none of the process are in memory
    }
    for(int i = 0; i < memorySize; i++) {
        memory[i] = -1; // Set memory to be empty
    }

    printf("******FIFO******\n");
    printf("Memory Status :\n");

    for(int i = 0; i < n; i++) {
        int currentPage = referenceString[i];
        if(pageTable[currentPage]) {	
        	printf("%d%d%d\t", memory[0], memory[1], memory[2]);
			continue;
        }

        int freeIndex = getFreeMemoryIndex(memory, memorySize);
        if(freeIndex == -1) {
            int oldest = deque();
            swapMemory(memory, memorySize, oldest, currentPage);

            enque(currentPage);
            pageTable[oldest] = 0;
            pageTable[currentPage] = 1;
        }
        else {
            memory[freeIndex] = currentPage;
            enque(currentPage);

            pageTable[currentPage] = 1;
        }
        pageFaults++;

        printf("%d%d%d\t", memory[0], memory[1], memory[2]);
    }

    printf("\nTotal no. of page faults : %d\n", pageFaults);
}

int main() {

	int referenceString[100];

	printf("Enter the number of pages: ");
	scanf("%d", &pages);

	printf("Enter the number of frames in memory: ");
	scanf("%d", &memorySize);

	printf("Enter page reference string for a process with 8 pages(String is terminated by -1) : ");
	
	int n = 0;
	while(1) {
		int s;
		scanf("%d", &s);

        if(s == -1) {
            break;
        }

        referenceString[n++] = s;
    }

    getMemoryStatusFIFO(referenceString, n);
}
