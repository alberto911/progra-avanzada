#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

static pthread_barrier_t barrier;


void* action(void* p) {
	int i;
	int id = (intptr_t) p;
	for (i = 0; i < 3; ++i) {
		printf("Hilo %d ejecutando\n", id);
		sleep(rand() % 5);
	}
	pthread_barrier_wait(&barrier);
	printf("Hilo %d terminó\n", id);	

	pthread_exit(NULL);	
}

int main() {
	srand(time(NULL));
	pthread_barrier_init(&barrier, NULL, 5);
	pthread_t threads[5];
	int i;

	for (i = 0; i < 5; ++i)
		pthread_create(&threads[i], NULL, action, (void*) (intptr_t) i);

	for (i = 0; i < 5; ++i)
		pthread_join(threads[i], NULL);

	return 0;
}
