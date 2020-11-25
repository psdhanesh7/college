#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t chopsticks[5];
sem_t mutex;

int philosopherIds[5] = { 0, 1, 2, 3, 4 };

void *philosopher(void *id) {
	int i = *(int *)id;

	while(1) {
		
		printf("Philosopher %d is thinking\n", i + 1);
		sleep(1);
		
		sem_wait(&mutex);

		sem_wait(&chopsticks[i]);
		sem_wait(&chopsticks[(i+1) % 5]);

		printf("Philosopher %d is eating\n", i + 1);
		sleep(1);

		sem_post(&chopsticks[i]);
		sem_post(&chopsticks[(i+1) % 5]);

		sem_post(&mutex);
	}
}

void main() {
	
	sem_init(&mutex, 0, 1);
	for(int i = 0; i < 5; i++) {
		sem_init(&chopsticks[i], 0, 1);
	}

	pthread_t threadId[5];
	for(int i = 0; i < 5; i++) {
		pthread_create(&threadId[i], NULL, philosopher, (void *) &philosopherIds[i]);
	}

	for(int i = 0; i < 5; i++) {
		pthread_join(threadId[i], NULL);
	}
}
