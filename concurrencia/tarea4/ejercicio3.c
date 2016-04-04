#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define N 3 // Número de secciones
#define M 5 // Número de robots

int secciones[N];
int pesos[N];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t libre[N];

void* comprar(void*);

int main() {
	int i;
	for (i = 0; i < N; ++i) {
		pthread_cond_init(&libre[i], NULL);
		secciones[i] = 10;
		pesos[i] = 0;
	}	

	srand((int) time(NULL));
	pthread_t robots[M];
	for (i = 0; i < M; ++i) {
		sleep(rand() % 5);
		pthread_create(&robots[i], NULL, comprar, (void*) (intptr_t) i);
	}

	for (i = 0; i < M; ++i) {
		pthread_join(robots[i], NULL);
	}
}

void* comprar(void* p) {
	int id = (intptr_t) p, i;
	int peso = (rand() % 10) + 1;
	printf("Llegó robot %d con peso %d\n", id, peso);

	for (i = 0; i < N; ++i) {
		pthread_mutex_lock(&mutex);
		while (pesos[i] + peso > secciones[i]) {
			printf("Robot %d esperando en sección %d\n", id, i);
			pthread_cond_wait(&libre[i], &mutex);
		}
	
		printf("Robot %d entrando a sección %d\n", id, i);
		pesos[i] += peso;
		pthread_mutex_unlock(&mutex);

		sleep(rand() % 10);
		pthread_mutex_lock(&mutex);
		printf("Robot %d saliendo de sección %d\n", id, i);
		pesos[i] -= peso;
		pthread_cond_broadcast(&libre[i]);
		pthread_mutex_unlock(&mutex);
	}

	printf("Se fue robot %d\n", id);
}

