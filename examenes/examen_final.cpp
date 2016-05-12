#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>

#define N 40 
#define P 10
#define PROB 2

int mapa[N/P][N];
int costos[P];
int size = N / P;

void* laberinto(void*);
void manejador(int);
int buscarCamino(int i, int j, int end, int costo);

int main() {
	int i, j;
	srand((int) time(NULL));

	signal(SIGUSR1, manejador);
	
	pthread_t procesadores[P];
	for (i = 0; i < P; ++i)
		pthread_create(&procesadores[i], NULL, laberinto, (void*) (intptr_t) i);

	for (i = 0; i < P; ++i)
		pthread_join(procesadores[i], NULL);

	int total = 0;
	for (i = 0; i < P; ++i) {
		total += costos[i];
	}
	printf("Costo total: %d\n\n", total);

	for (i = 0; i < size; ++i) {
		for (j = 0; j < N; ++j)
			printf("%d ", mapa[i][j]);
		printf("\n");
	}

	return 0;
}

void* laberinto(void* p) {
	int id = (intptr_t) p, i, j;
	unsigned int seed = rand();
	int start = id * size;
	int end = (id + 1) * size;

	for (i = 0; i < size; ++i) {
		for (j = start; j < end; ++j) {
			if ((rand_r(&seed) % 10 < PROB) && !(i == 0 && j == start) && !(i == (size-1) && j == (end-1)))
				mapa[i][j] = 1;
			else			
				mapa[i][j] = 0;
		}
	}

	costos[id] = buscarCamino(0, start, end, 1);
}

int buscarCamino(int i, int j, int end, int costo) {
	mapa[i][j] = 2;
	if (i == (size-1) && j == (end-1))
		return costo;
	else if ((i+1) < size && (j+1) < end && !(mapa[i+1][j+1]))
		buscarCamino(i+1, j+1, end, costo+1);
	else if ((i+1) < size && !(mapa[i+1][j]))
		buscarCamino(i+1, j, end, costo+1);
	else if ((j+1) < end && !(mapa[i][j+1]))
		buscarCamino(i, j+1, end, costo+1);
}

void manejador(int s) {
	int i, j;
	printf("Resultado parcial:\n");

	for (i = 0; i < size; ++i) {
		for (j = 0; j < N; ++j)
			printf("%d ", mapa[i][j]);
		printf("\n");
	}
}

