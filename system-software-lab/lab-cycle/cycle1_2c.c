//Author: Dhruv Eldho Peter
//Date: 08/09/2000
//Program: 2c
//C-SCAN Scheduling Algorithm

#include<stdio.h>
#include<stdlib.h>

#define distStart 0
#define distEnd 199

int comparator(const void *p, const void *q) { 
    // Get the values at given addresses 
    int l = *(const int *)p; 
    int r = *(const int *)q; 

    return l-r;
} 

//Function to print seek time
void printHeadPtrMovementCSCAN(int rQueue[], int n, int headPtr) {

    int seekTime = 0;
    int currentPtr = headPtr;
    int nextPtr;
    int r = n; 

    qsort((void *)rQueue, n, sizeof(rQueue[0]), comparator);

    int start = 0;
    while(rQueue[start] <= headPtr) {
        start++;
    }

    printf("*-----------------C-SCAN------------------\n");
    printf("Header Movement : %d", headPtr);

    int i = start;
    while(i < n && r) {
        nextPtr = rQueue[i];
        printf(" -> %d", nextPtr);

        seekTime += nextPtr - currentPtr;
        currentPtr = nextPtr;

        i++;
        r--;
    }

    if(r) {
        nextPtr = distEnd;
        printf(" -> %d -> %d", distEnd, distStart);

		seekTime += (2 * distEnd - distStart - currentPtr);

		currentPtr = 0;
    }

    i = 0;
    while(r--) {
        nextPtr = rQueue[i];
        printf(" -> %d", nextPtr);

        seekTime += nextPtr - currentPtr;
        currentPtr = nextPtr;

        i++;
    }

    printf("\nTotal seek time : %d\n", seekTime);
   
}

void main() {
    int rQueue[100];
    int n = 0; 
    int headPtr;

    printf("Enter the request sequence(Sequence is terminated by -1) : ");

    while(1) {
        int r;
        scanf("%d", &r);

        if(r == -1) {
            break;
        }

        rQueue[n++] = r;
    }

    printf("Enter the initial head position : ");
    scanf("%d", &headPtr);

    printHeadPtrMovementCSCAN(rQueue, n, headPtr);
}
