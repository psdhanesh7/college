// Author : Dhanesh P S
// Date : 08/09/2020

// Problem Statement : Implement disk scheduling algorithm - FIFO 

#include<stdio.h>
#include<stdlib.h>

#define distStart 0
#define distEnd 199

void printHeadPointerMovementFIFO(int requestQueue[], int n, int headPointer) {

    int headMovement = 0;
    int currentPointer = headPointer;

    printf("******FIFO******\n");
    printf("Header Movement : %d", headPointer);

    for(int i = 0; i < n; i++) {
        int nextPointer = requestQueue[i];
        printf(" -> %d", nextPointer);

        headMovement += abs(nextPointer - currentPointer);
        currentPointer = nextPointer;
    }

    printf("\nTotal Head Movemeny : %d\n", headMovement);
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

    printHeadPointerMovementFIFO(requestQueue, n, headPointer);
}

// Input

// 176 79 34 60 92 11 41 114 -1
// 50

