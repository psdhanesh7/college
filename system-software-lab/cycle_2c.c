// Author : Dhanesh P S
// Date : 08/09/2020

// Problem Statement : Implement page replacement algorithms -  LFU

#include<stdio.h>

#define memorySize 3
#define pages 8

typedef struct {
	
	int pageNo;
	int frequency;
	int lastIndex;
};

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

int getLeastFrequentlyUsed(int frequency[], int memory[], int n) {

    int lfu = memory[0];
    for(int i = 0; i < n; i++) {
        if(frequency[memory[i]] < frequency[lfu]) {
            lfu = memory[i];
        }
    }
    return lfu;
}

void getMemoryStatusLFU(int referenceString[], int n) {

    int pageTable[10];
    int memory[memorySize]; // No. of frames is assumed to be 3
    int pageFaults = 0;
    int frequency[10];

    for(int i = 0; i < pages; i++) { // No. of pages is assumed to be 8
        pageTable[i] = 0; // Initailly none of the process are in memory
		frequency[i] = 0; // None of the processes are used yet
    }
    for(int i = 0; i < memorySize; i++) {
        memory[i] = -1; // Set memory to be empty
    }

    printf("*****LFU*****\n");
    printf("Memory Status : ");

    for(int i = 0; i < n; i++) {
        int currentPage = referenceString[i];
        if(pageTable[currentPage]) {
            frequency[currentPage]++;
            continue;
        }

        int freeIndex = getFreeMemoryIndex(memory, memorySize);
        if(freeIndex == -1) {

            int lfu = getLeastFrequentlyUsed(frequency, memory, memorySize);
            swapMemory(memory, memorySize, lfu, currentPage);

            pageTable[lfu] = 0;
            pageTable[currentPage] = 1;

            frequency[currentPage]++;
        }
        else {
            memory[freeIndex] = currentPage;

            pageTable[currentPage] = 1;
            frequency[currentPage]++;
        }
        pageFaults++;

        printf("%d%d%d\t", memory[0], memory[1], memory[2]);
    }

    printf("\nTotal no. of page faults : %d\n", pageFaults);
}

int main() {

    int referenceString[100];

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

    getMemoryStatusLFU(referenceString, n);
}
