#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define N 3

int alarmas = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t libreAlarma = PTHREAD_COND_INITIALIZER;

void* escucharDatos(void*);
void* escucharAlarma(void*);
void manejador(int);

int main() {
	int i;
	srand((int) time(NULL));
	pthread_t registrosDatos[N];
	pthread_t registrosAlarma[N];
	for (i = 0; i < N; ++i) {
		pthread_create(&registrosDatos[i], NULL, escucharDatos, (void*) (intptr_t) i);
		pthread_create(&registrosAlarma[i], NULL, escucharAlarma, (void*) (intptr_t) i);
	}

	for (i = 0; i < N; ++i) {
		pthread_join(registrosDatos[i], NULL);
		pthread_join(registrosAlarma[i], NULL);
	}
}

void* escucharDatos(void* p) {
	int id = (intptr_t) p, i;
	while (1) {
		sleep(10);
	}
}

void* escucharAlarma(void* p) {
	int id = (intptr_t) p, i;
	while (1) {
	}
}

