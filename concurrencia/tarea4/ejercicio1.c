#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define N 7
#define S 4
#define M 2

int sentados = 0;
int sentadosSinServir = 0;
int total = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t servido = PTHREAD_COND_INITIALIZER;
pthread_cond_t libre = PTHREAD_COND_INITIALIZER;
pthread_cond_t porServir = PTHREAD_COND_INITIALIZER;

void* funcBlancanieves(void*);
void* funcEnanito(void*);

int main() {
	int i;
	
	srand((int) time(NULL));
	pthread_t blancanieves;
	pthread_create(&blancanieves, NULL, funcBlancanieves, NULL);
	pthread_t enanitos[N];
	for (i = 0; i < N; ++i) {
		pthread_create(&enanitos[i], NULL, funcEnanito, (void*) (intptr_t) i);
	}

	for (i = 0; i < N; ++i) {
		pthread_join(enanitos[i], NULL);
		++total;
	}
}

void* funcEnanito(void* p) {
	int i = 0;
	while (i++ < M) {
		sleep(rand() % 20);
		int id = (intptr_t) p;
		printf("Llegó enanito %d\n", id);

		pthread_mutex_lock(&mutex);	
		if (sentados == S)
			pthread_cond_wait(&libre, &mutex);

		printf("Se sentó enanito %d\n", id);
		sentados++;
		sentadosSinServir++;

		if (sentadosSinServir == 1)
			pthread_cond_signal(&porServir);

		pthread_cond_wait(&servido, &mutex);
		sentadosSinServir--;
		printf("Enanito %d comiendo\n", id);
		pthread_mutex_unlock(&mutex);
		sleep(10);	

		pthread_mutex_lock(&mutex);
		printf("Se fue enanito %d\n", id);
		sentados--;
		if (sentados == S-1)
			pthread_cond_signal(&libre);
		pthread_mutex_unlock(&mutex);
	}
}

void* funcBlancanieves(void* p) {
	while (total < N) {
		pthread_mutex_lock(&mutex);
		if (sentadosSinServir == 0) {
			printf("Blancanieves se fue\n");
			pthread_cond_wait(&porServir, &mutex);
			printf("Blancanieves volvió\n");
		}
		pthread_mutex_unlock(&mutex);

		sleep(5);
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&servido);
		pthread_mutex_unlock(&mutex);
	}
}

