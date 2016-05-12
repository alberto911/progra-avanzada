#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 10

int hombresEsperando = 0;
int mujeresEsperando = 0;
int hombresAdentro = 0;
int mujeresAdentro = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t libreHombres = PTHREAD_COND_INITIALIZER;
pthread_cond_t libreMujeres = PTHREAD_COND_INITIALIZER;

void* hombre(void*);
void* mujer(void*);

int main() {
	int i, tipo;
	printf("Sanitario vacío\n");	

	srand((int) time(NULL));
	pthread_t estudiantes[N];
	for (i = 0; i < N; ++i) {
		sleep(rand() % 5);
		tipo = rand() % 2;
		if (tipo)
			pthread_create(&estudiantes[i], NULL, hombre, NULL);
		else
			pthread_create(&estudiantes[i], NULL, mujer, NULL);
	}

	for (i = 0; i < N; ++i) {
		pthread_join(estudiantes[i], NULL);
	}
}

void* hombre(void* p) {
	pthread_mutex_lock(&mutex);
	printf("Llega un hombre (%d en espera)\n", ++hombresEsperando);
	
	if (mujeresAdentro)
		pthread_cond_wait(&libreHombres, &mutex);

	++hombresAdentro;
	if (hombresAdentro == 1)
		printf("Sanitario ocupado por hombres\n");

	printf("Entra un hombre (%d en espera)\n", --hombresEsperando);
	pthread_mutex_unlock(&mutex);

	sleep(10);
	pthread_mutex_lock(&mutex);
	--hombresAdentro;	
	printf("Sale un hombre\n");
	if (hombresAdentro == 0) {
		printf("Sanitario vacío\n");
		pthread_cond_broadcast(&libreMujeres);
	}
	pthread_mutex_unlock(&mutex);
}

void* mujer(void* p) {
	pthread_mutex_lock(&mutex);
	printf("Llega una mujer (%d en espera)\n", ++mujeresEsperando);
	
	if (hombresAdentro)
		pthread_cond_wait(&libreMujeres, &mutex);

	++mujeresAdentro;
	if (mujeresAdentro == 1)
		printf("Sanitario ocupado por mujeres\n");

	printf("Entra una mujer (%d en espera)\n", --mujeresEsperando);
	pthread_mutex_unlock(&mutex);

	sleep(10);
	pthread_mutex_lock(&mutex);
	--mujeresAdentro;	
	printf("Sale una mujer\n");
	if (mujeresAdentro == 0) {
		printf("Sanitario vacío\n");
		pthread_cond_broadcast(&libreHombres);
	}
	pthread_mutex_unlock(&mutex);
}

