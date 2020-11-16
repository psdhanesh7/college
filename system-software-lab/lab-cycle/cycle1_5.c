//Author: Dhruv Eldho Peter
//Date: 22/09/2020
//Program Name: Producer-Consumer Problem


#include <stdio.h>
#include <stdlib.h>

#define n 3

typedef struct {
	int v;
} semaphore;

int buffer[n];
int in = 0;
int out = 0;

semaphore mutex;
semaphore full;
semaphore empty;

void wait(semaphore *s) {
	s->v--;
}

void signal(semaphore *s) {
	s->v++;
}

void p() {			//Producer function
	
	int newItem = rand(); // Producing a new item

	wait(&empty);	// Wait until empty > 0
	if(empty.v < 0) {
		printf("Buffer is full!!!\n");
		empty.v++;
		return;
	}
	wait(&mutex); // Acquiring lock

	buffer[in] = newItem; // Store produced item in buffer
	in = (in + 1)%n;
	printf("Producer produces an item\n");

	signal(&mutex); // Release lock
	signal(&full);
}

void c() {		//Consumer function
	
	wait(&full);
	if(full.v < 0) {
		printf("Buffer is empty!!\n");
		full.v++;
		return;
	}
	wait(&mutex); // Aquiring mutex lock

	int newItem = buffer[out]; // Consuming an item
	out = (out + 1)%n;
	printf("Consumer consumes an item\n");
	
	signal(&mutex); // Releasing the mutex lock
	signal(&empty);

}

void main() {
	
	mutex.v = 1;
	empty.v = n;
	full.v = 0;

	int a = 1;
	printf("1. Producer\n");
	printf("2. Consumer\n");
	printf("3. Exit\n");
	
	int choice;
	while(a) {
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch(choice) {
			case 1:
				p();
				break;
			case 2:
				c();
				break;
			case 3:
				a = 0;
				break;
			default:
				printf("Invalid Choice!!\n");
		}
	}
}
