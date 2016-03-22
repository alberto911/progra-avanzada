#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define NG 100
#define NE 50
#define M 5

typedef struct {
	pthread_mutex_t* mutex;
	pthread_cond_t* var;
	int wait_general;
	int wait_empresarial;
} params;

sem_t sem_generales;
sem_t sem_empresariales;

int cajeros_generales[5];
int generales_disponibles[5];
int cajeros_empresariales[3];
int empresariales_disponibles[3];

pthread_mutex_t mutex_generales;
pthread_mutex_t mutex_empresariales;

void* operaciones_generales(void*);
void* operaciones_empresariales(void*);
void* crear_generales(void*);
void* crear_empresariales(void*);
void* wait_g(void*);
void* wait_e(void*);

int main() {
	sem_init(&sem_generales, 0, 5);
	sem_init(&sem_empresariales, 0, 3);

	srand((int) time(NULL));
	pthread_t creador_generales, creador_empresariales, verificador;
	pthread_create(&creador_generales, NULL, crear_generales, NULL);
	pthread_create(&creador_empresariales, NULL, crear_empresariales, NULL);
	
	int i;

	// Inicializar contadores de cajeros
	for (i = 0; i < 5; ++i) {
		cajeros_generales[i] = M;
		generales_disponibles[i] = 1;
	}
	for (i = 0; i < 3; ++i) {
		cajeros_empresariales[i] = M;
		empresariales_disponibles[i] = 1;
	}

	pthread_join(creador_generales, NULL);
	pthread_join(creador_empresariales, NULL);

	sem_destroy(&sem_generales);
	sem_destroy(&sem_empresariales);
	return 0;
}

void* crear_generales(void* p) {
	pthread_t threads_generales[NG], *aux;
	int i;

	for (aux = threads_generales, i = 1; aux < (threads_generales + NG); ++aux, ++i) {
		sleep((rand() % 18) + 5);
		pthread_create(aux, NULL, operaciones_generales, (void*) (intptr_t) i);
	}

	for (aux = threads_generales; aux < (threads_generales + NG); ++aux) {
		pthread_join(*aux, NULL);
	}

	pthread_exit(NULL);
}

void* crear_empresariales(void* p) {
	pthread_t threads_empresariales[NE], *aux;
	int i;
	
	for (aux = threads_empresariales, i = NG + 1; aux < (threads_empresariales + NE); ++aux, ++i) {
		sleep((rand() % 26) + 9);
		pthread_create(aux, NULL, operaciones_empresariales, (void*) (intptr_t) i);
	}

	for (aux = threads_empresariales; aux < (threads_empresariales + NE); ++aux) {
		pthread_join(*aux, NULL);
	}

	pthread_exit(NULL);
}

void* wait_g(void* p) {
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	sleep(1);
	sem_wait(&sem_generales);
	params* parametros = (params*) p;
	pthread_mutex_lock(parametros->mutex);
	parametros->wait_general = 1;
	pthread_cond_signal(parametros->var);
	pthread_mutex_unlock(parametros->mutex);
	pthread_exit(NULL);
}

void* wait_e(void* p) {
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	sleep(2);
	sem_wait(&sem_empresariales);
	params* parametros = (params*) p;
	pthread_mutex_lock(parametros->mutex);
	parametros->wait_empresarial = 1;
	pthread_cond_signal(parametros->var);
	pthread_mutex_unlock(parametros->mutex);
	pthread_exit(NULL);
}

void* operaciones_generales(void* p) {
	int id = (intptr_t) p, i = 0, cajero, temp_status;
	printf("Llegó el cliente %d. Operación general\n", id);

	pthread_mutex_t mutex;
	pthread_cond_t var;
	pthread_t espera_g, espera_e;

	pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&var, NULL);

	params parametros;
	parametros.mutex = &mutex;
	parametros.var = &var;
	parametros.wait_general = 0;
	parametros.wait_empresarial = 0;

	pthread_create(&espera_g, NULL, wait_g, &parametros);
	pthread_create(&espera_e, NULL, wait_e, &parametros);

	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&var, &mutex);
	temp_status = parametros.wait_general;
	if (temp_status == 1) {
		pthread_cancel(espera_e);
		if (parametros.wait_empresarial)
			sem_post(&sem_empresariales);
	}
	else
		pthread_cancel(espera_g);
	pthread_mutex_unlock(&mutex);

	if (temp_status) {
		pthread_mutex_lock(&mutex_generales);
		while (!generales_disponibles[i])
			++i;
		generales_disponibles[i] = 0;
		cajero = i;
		pthread_mutex_unlock(&mutex_generales);

		printf("Atendiendo al cliente %d en el cajero G%d. Operación general\n", id, cajero);
		sleep((rand() % 19) + 12);

		cajeros_generales[cajero]--;
		if (cajeros_generales[cajero] == 0) {
			printf("Cajero G%d en espera\n", cajero);
			sleep(18);
			cajeros_generales[cajero] = M;
		}

		pthread_mutex_lock(&mutex_generales);
		generales_disponibles[cajero] = 1;
		pthread_mutex_unlock(&mutex_generales);
		sem_post(&sem_generales);
	}
	else {
		pthread_mutex_lock(&mutex_empresariales);
		while (!empresariales_disponibles[i])
			++i;
		empresariales_disponibles[i] = 0;
		cajero = i;
		pthread_mutex_unlock(&mutex_empresariales);

		printf("Atendiendo al cliente %d en el cajero E%d. Operación general\n", id, cajero);
		sleep((rand() % 19) + 12);

		cajeros_empresariales[cajero]--;
		if (cajeros_empresariales[cajero] == 0) {
			printf("Cajero E%d en espera\n", cajero);
			sleep(18);
			cajeros_empresariales[cajero] = M;
		}

		pthread_mutex_lock(&mutex_empresariales);
		empresariales_disponibles[cajero] = 1;
		pthread_mutex_unlock(&mutex_empresariales);
		sem_post(&sem_empresariales);
	}

	pthread_exit(NULL);
}

void* operaciones_empresariales(void* p) {
	int id = (intptr_t) p, i = 0, cajero;
	printf("Llegó el cliente %d. Operación empresarial\n", id);

	sem_wait(&sem_empresariales);

	pthread_mutex_lock(&mutex_empresariales);
	while (!empresariales_disponibles[i])
		++i;
	empresariales_disponibles[i] = 0;
	cajero = i;
	pthread_mutex_unlock(&mutex_empresariales);

	printf("Atendiendo al cliente %d en el cajero E%d. Operación empresarial\n", id, cajero);
	sleep((rand() % 19) + 12);

	cajeros_empresariales[cajero]--;
	if (cajeros_empresariales[cajero] == 0) {
		printf("Cajero E%d en espera\n", cajero);
		sleep(18);
		cajeros_empresariales[cajero] = M;
	}

	pthread_mutex_lock(&mutex_empresariales);
	empresariales_disponibles[cajero] = 1;
	pthread_mutex_unlock(&mutex_empresariales);

	sem_post(&sem_empresariales);

	pthread_exit(NULL);
}
