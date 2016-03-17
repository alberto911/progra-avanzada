#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

sem_t sem_sillas;
sem_t sem_tenedores;
sem_t sem_cuchillos;

void* comer(void*);

int main() {
	sem_init(&sem_sillas, 0, 4);
	sem_init(&sem_tenedores, 0, 3);
	sem_init(&sem_cuchillos, 0, 3);

	srand((int) time(NULL));
	pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));
	pthread_t* aux;
	int i;

	for (aux = threads, i = 0; aux < (threads + N); ++aux, ++i) {
		pthread_create(aux, NULL, comer, (void*) i);
	}

	for (aux = threads; aux < (threads + N); ++aux) {
		pthread_join(*aux, NULL);
	}

	sem_destroy(&sem_sillas);
	sem_destroy(&sem_tenedores);
	sem_destroy(&sem_cuchillos);
	free(threads);
	return 0;
}

void* comer(void* p) {
	int id = (int) p;
	sem_wait(&sem_sillas);
	printf("Ya estoy sentado %d\n", id);
	sem_wait(&sem_tenedores);
	printf("Ya tengo un tenedor %d\n", id);
	sem_wait(&sem_cuchillos);
	printf("Ya tengo un cuchillo. Comiendo... %d\n", id);
	sleep((rand() % 16) + 5);
	printf("Acabé de comer %d\n", id);
	sem_post(&sem_cuchillos);
	sem_post(&sem_tenedores);

	sem_post(&sem_sillas);	
	printf("Ya me paré %d\n", id);

	pthread_exit(NULL);
}
