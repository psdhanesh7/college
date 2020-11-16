// Author : Dhanesh P S
// Date : 08/09/2020

// Problem Statement : Implement page replacement algorithms -  LRU

#include<stdio.h>

int memorySize;
int pages;

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

int getLeastRecentlyUsed(int recenltyUsedIndex[], int memory[], int n) {

    int lru = memory[0];
    for(int i = 0; i < n; i++) {
        if(recenltyUsedIndex[memory[i]] < recenltyUsedIndex[lru]) {
            lru = memory[i];
        }
    }
    return lru;
}

void getMemoryStatusLRU(int referenceString[], int n) {

    int pageTable[10];
    int memory[memorySize]; // No. of frames is assumed to be 3
    int pageFaults = 0;
    int recentlyUsedIndex[10];

    for(int i = 0; i < pages; i++) { // No. of pages is assumed to be 8
        pageTable[i] = 0; // Initailly none of the process are in memory
    }
    for(int i = 0; i < memorySize; i++) {
        memory[i] = -1; // Set memory to be empty
    }

    printf("*****LRU*****\n");
    printf("Memory Status :\n");

    for(int i = 0; i < n; i++) {
        int currentPage = referenceString[i];
        if(pageTable[currentPage]) {
        	printf("%d%d%d\t", memory[0], memory[1], memory[2]);
		   	recentlyUsedIndex[currentPage] = i;
            continue;
        }

        int freeIndex = getFreeMemoryIndex(memory, memorySize);
        if(freeIndex == -1) {

            int lru = getLeastRecentlyUsed(recentlyUsedIndex, memory, memorySize);
            swapMemory(memory, memorySize, lru, currentPage);

            pageTable[lru] = 0;
            pageTable[currentPage] = 1;

            recentlyUsedIndex[currentPage] = i;
        }
        else {
            memory[freeIndex] = currentPage;

            pageTable[currentPage] = 1;
            recentlyUsedIndex[currentPage] = i;
        }
        pageFaults++;

        printf("%d%d%d\t", memory[0], memory[1], memory[2]);
    }

    printf("\nTotal no. of page faults : %d\n", pageFaults);
}

int main() {

    int referenceString[100];
	   
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

    getMemoryStatusLRU(referenceString, n);
}
