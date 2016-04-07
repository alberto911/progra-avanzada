#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define N 3 // Número de fumadores

int fumando[N];
int recursos[N];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* administrar(void*);
void* fumador(void*);

int main() {
	int i;
	for (i = 0; i < N; ++i) {
		fumando[i] = 0;
		recursos[i] = 0;
	}

	srand((int) time(NULL));
	pthread_t agente;
	pthread_create(&agente, NULL, administrar, NULL);

	pthread_t fumadores[N];
	for (i = 0; i < N; ++i)
		pthread_create(&fumadores[i], NULL, fumador, (void*) (intptr_t) i);

	for (i = 0; i < N; ++i)
		pthread_join(fumadores[i], NULL);
	
	pthread_join(agente, NULL);

	return 0;
}

void* administrar(void* p) {
	int i;
	for (i = 0; i < N; i = (i+1) % N) {		
		pthread_mutex_lock(&mutex);
		if (!fumando[i]) {
			pthread_mutex_unlock(&mutex);
			sleep(15);
			pthread_mutex_lock(&mutex);
			recursos[i] = 1;
			if (i == 0)
				printf("Papel en la mesa\n");
			else if (i == 1)
				printf("Tabaco en la mesa\n");
			else
				printf("Cerillos en la mesa\n");
		}
		pthread_mutex_unlock(&mutex);
	} 
}

void* fumador(void* p) {
	int id = (intptr_t) p;
	while (1) {
		pthread_mutex_lock(&mutex);
		if (recursos[0] && recursos[1] && recursos[2]) {
			recursos[0] = recursos[1] = recursos[2] = 0;
			fumando[id] = 1;
			printf("Fumador %d fumando\n", id);
			pthread_mutex_unlock(&mutex);
			sleep(10);
			printf("Fumador %d acabó de fumar\n", id);
			pthread_mutex_lock(&mutex);
			fumando[id] = 0;
			pthread_mutex_unlock(&mutex);
			sleep(20);
		}
		else
			pthread_mutex_unlock(&mutex);
	}
}

