#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
#define W 15

sem_t semEste;
sem_t semOeste;

pthread_mutex_t mutexEste;
pthread_mutex_t mutexOeste;

void* cruzarEste(void*);
void* cruzarOeste(void*);

int main() {
	sem_init(&semEste, 0, 0);
	sem_init(&semOeste, 0, 0);

	srand((int) time(NULL));
	pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));
	pthread_t* aux;
	int i, direccion;

	for (aux = threads, i = 0; aux < (threads + N); ++aux, ++i) {
		direccion = rand() % 2;
		if (direccion)
			pthread_create(aux, NULL, cruzarOeste, (void*) i);
		else
			pthread_create(aux, NULL, cruzarEste, (void*) i);
	}

	for (aux = threads; aux < (threads + N); ++aux) {
		pthread_join(*aux, NULL);
	}

	sem_destroy(&semEste);
	sem_destroy(&semOeste);
	free(threads);
	return 0;
}

void* cruzarEste(void* p) {
	int id = (int) p;
	sleep(rand() % W);

	int valOeste, continuar = 1;
	while (continuar) {
		pthread_mutex_lock(&mutexOeste);

		sem_getvalue(&semOeste, &valOeste);
		if (!valOeste) {
			while (pthread_mutex_trylock(&mutexEste)) {
				pthread_mutex_unlock(&mutexOeste);
				pthread_mutex_lock(&mutexOeste);
			}
			sem_post(&semEste);
			pthread_mutex_unlock(&mutexEste);
			continuar = 0;
		}
		
		pthread_mutex_unlock(&mutexOeste);
	}
		
	printf("Babuino %d cruzando hacia el este\n", id);
	sleep(3);
	printf("Babuino %d llegó al este\n", id);
	sem_wait(&semEste);
	pthread_exit(NULL);
}

void* cruzarOeste(void* p) {
	int id = (int) p;
	sleep(rand() % W);

	int valEste, continuar = 1;
	while (continuar) {
		pthread_mutex_lock(&mutexEste);

		sem_getvalue(&semEste, &valEste);
		if (!valEste) {
			while (pthread_mutex_trylock(&mutexOeste)) {
				pthread_mutex_unlock(&mutexEste);
				pthread_mutex_lock(&mutexEste);
			}
			sem_post(&semOeste);
			pthread_mutex_unlock(&mutexOeste);
			continuar = 0;
		}
		
		pthread_mutex_unlock(&mutexEste);
	}
	
	printf("Babuino %d cruzando hacia el oeste\n", id);
	sleep(3);
	printf("Babuino %d llegó al oeste\n", id);
	sem_wait(&semOeste);
	pthread_exit(NULL);
}
