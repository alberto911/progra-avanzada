#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define C 10
#define S 3
#define T 3
#define U 50
#define N 1000

sem_t* taquillas;
sem_t** salas;
int total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inicia_pelicula = PTHREAD_COND_INITIALIZER;

void* comprarBoleto(void*);
void* reloj(void*);

int main() {
	int i, j;
	taquillas = (sem_t*) malloc (C * sizeof(sem_t));
	salas = (sem_t**) malloc (C * sizeof(sem_t*));
	
	// Crear los semáforos
	for (i = 0; i < C; ++i) {
		sem_init(&taquillas[i], 0, T);
		salas[i] = (sem_t*) malloc (S * sizeof(sem_t));
		for (j = 0; j < S; ++j)
			sem_init(&salas[i][j], 0, U);
	}

	srand((int) time(NULL));
	pthread_t thread_reloj;
	pthread_create(&thread_reloj, NULL, reloj, NULL);
	pthread_t threads[N];
	for (i = 0; i < N; ++i) {
		sleep(rand() % 5);
		pthread_create(&threads[i], NULL, comprarBoleto, (void*) (intptr_t) i);
	}

	for (i = 0; i < N; ++i) {
		pthread_join(threads[i], NULL);
		++total;
	}

	// Liberar los semáforos
	for (i = 0; i < C; ++i) {
		sem_destroy(&taquillas[i]);
		for (j = 0; j < S; ++j)
			sem_destroy(&salas[i][j]);
		free(salas[i]);
	}
	free(salas);
	free(taquillas);
}

void* comprarBoleto(void* p) {
	int id = (intptr_t) p;
	int complejo = rand() % C;
	int sala = rand() % S;
	printf("Cliente %d esperando en taquilla. Complejo: %d, Sala: %d\n", id, complejo, sala);
	sem_wait(&taquillas[complejo]);
	printf("Cliente %d comprando boleto. Complejo: %d, Sala: %d\n", id, complejo, sala);
	sleep(5);
	printf("Cliente %d compró boleto. Complejo: %d, Sala: %d\n", id, complejo, sala);
	sem_post(&taquillas[complejo]);

	sem_wait(&salas[complejo][sala]);
	printf("Cliente %d esperando inicio. Complejo: %d, Sala: %d\n", id, complejo, sala);

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&inicia_pelicula, &mutex);
	pthread_mutex_unlock(&mutex);

	printf("Cliente %d viendo película. Complejo: %d, Sala: %d\n", id, complejo, sala);
	sleep(30);
	sem_post(&salas[complejo][sala]);
	printf("Cliente %d salió. Complejo: %d, Sala: %d\n", id, complejo, sala);
}

void* reloj(void* p) {
	while (total < N) {
		sleep(60);
		pthread_mutex_lock(&mutex);
		printf("\n\nInicia la película\n\n");
		pthread_cond_broadcast(&inicia_pelicula);
		pthread_mutex_unlock(&mutex);
	}
}

