#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define N 3

typedef struct {
	int id;
	int* datos;
	pthread_cond_t* nuevosDatos;
	pthread_mutex_t* mutexDatos;
} params_datos_t;

int alarmas = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t libreDatos = PTHREAD_COND_INITIALIZER;

void* leerDatos(void*);
void* leerAlarma(void*);
void* escribirAlarma(void*);
void* escribirDatos(void*);
void manejador(int);

int main() {
	int i;
	srand((int) time(NULL));
	pthread_t registrosDatos[N];
	pthread_t registrosAlarma[N];
	for (i = 0; i < N; ++i) {
		pthread_create(&registrosDatos[i], NULL, leerDatos, (void*) (intptr_t) i);
		pthread_create(&registrosAlarma[i], NULL, leerAlarma, (void*) (intptr_t) i);
	}

	for (i = 0; i < N; ++i) {
		pthread_join(registrosDatos[i], NULL);
		pthread_join(registrosAlarma[i], NULL);
	}
}

void* leerDatos(void* p) {
	int id = (intptr_t) p, datos = 0;
	pthread_t thread;
	pthread_mutex_t mutexDatos = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t nuevosDatos = PTHREAD_COND_INITIALIZER;

	params_datos_t params;
	params.id = id;
	params.mutexDatos = &mutexDatos;
	params.nuevosDatos = &nuevosDatos;
	params.datos = &datos;

	pthread_create(&thread, NULL, escribirDatos, (void*) &params);

	while (1) {
		sleep(5); // Cada 5 segundos se hace una lectura
		pthread_mutex_lock(&mutexDatos);
		datos++;
		printf("Nuevos datos UAD %d\n", id);
		pthread_cond_signal(&nuevosDatos);
		pthread_mutex_unlock(&mutexDatos);
	}
	pthread_join(thread, NULL);
}

void* leerAlarma(void* p) {
	int id = (intptr_t) p;
	pthread_t thread;
	while (1) {
		sleep(rand() % 10); // Tiempo aleatorio de alarma
		pthread_mutex_lock(&mutex);
		++alarmas;
		printf("%d alarmas pendientes\n", alarmas);
		pthread_create(&thread, NULL, escribirAlarma, (void*) (intptr_t) id);
		pthread_mutex_unlock(&mutex);
	}
}

void* escribirAlarma(void* p) {
	int id = (intptr_t) p;
	pthread_mutex_lock(&mutex);
	printf("Escribiendo alarma UAD %d\n", id);
	sleep(1); // Tiempo que tarda en escribir
	--alarmas;
	printf("%d alarmas pendientes\n", alarmas);
	if (alarmas == 0)
		pthread_cond_broadcast(&libreDatos);
	pthread_mutex_unlock(&mutex);
}

void* escribirDatos(void* p) {
	int id = ((params_datos_t*) p)->id;

	while (1) {
		pthread_mutex_lock(((params_datos_t*) p)->mutexDatos);
		pthread_cond_wait( ((params_datos_t*) p)->nuevosDatos, ((params_datos_t*) p)->mutexDatos );
		pthread_mutex_unlock(((params_datos_t*) p)->mutexDatos);
		
		pthread_mutex_lock(&mutex);
		while (alarmas)
			pthread_cond_wait(&libreDatos, &mutex);

		pthread_mutex_lock(((params_datos_t*) p)->mutexDatos);
		printf("Escribiendo datos UAD %d: %d\n", id, *(((params_datos_t*) p)->datos) );
		sleep(1);
		pthread_mutex_unlock(((params_datos_t*) p)->mutexDatos);
		pthread_mutex_unlock(&mutex);
	}
}

