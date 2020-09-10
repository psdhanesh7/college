// Author : Dhanesh P S
// Date : 08/09/2020

// Problem Statement : Implement disk scheduling algorithm - SCAN

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

void printHeadPointerMovementSCAN(int requestQueue[], int n, int headPointer) {

    int headMovement = 0;
    int currentPointer = headPointer;
    int nextPointer;
    int requests = n; 

    qsort((void *)requestQueue, n, sizeof(requestQueue[0]), comparator);

    int start = 0;
    while(requestQueue[start] <= headPointer) {
        start++;
    }

    printf("******SCAN******\n");
    printf("Header Movement : %d", headPointer);

    int i = start;
    while(i < n && requests) {
        nextPointer = requestQueue[i];
        printf(" -> %d", nextPointer);

        headMovement += nextPointer - currentPointer;
        currentPointer = nextPointer;

        i++;
        requests--;
    }

    if(requests) {
        nextPointer = distEnd;
        printf(" -> %d", nextPointer);

        headMovement += nextPointer - currentPointer;
        currentPointer = nextPointer;
    }

    i = start - 1;
    while(requests--) {
        nextPointer = requestQueue[i];
        printf(" -> %d", nextPointer);

        headMovement += currentPointer - nextPointer;
        currentPointer = nextPointer;

        i--;
    }

    printf("\nTotal head movement : %d\n", headMovement);
}

void main() {
    int requestQueue[100];
    int n = 0; 
    int headPointer;

    printf("Enter the request sequence(Sequence is terminated by -1) : ");

    while(1) {
        int request;
        scanf("%d", &request);

        if(request == -1) {
            break;
        }

        requestQueue[n++] = request;
    }

    printf("Enter the head pointer : ");
    scanf("%d", &headPointer);

    printHeadPointerMovementSCAN(requestQueue, n, headPointer);
}

