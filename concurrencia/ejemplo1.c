/* semt_t
   sem_init(sem_t*, 0, 5)
   sem_wait(sem_t*)
   sem_getvalue(sem_t*)
   sem_post(sem_t*)
   sem_destroy(sem_t*) */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int numero = 10;

pthread_mutex_t mutex;

typedef struct {
	int n;
} params;

void* duplicar (void* p) {
	int i;
	for (i = 0; i < ((params*)p)->n; ++i) {
		pthread_mutex_lock(&mutex);
		numero *= 2;
		printf("Duplica: %d\n", numero);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	pthread_exit(NULL);
}

void* dividir (void* p) {
	int i;
	for (i = 0; i < ((params*)p)->n; ++i) {
		pthread_mutex_lock(&mutex);
		numero /= 2;
		printf("Divide: %d\n", numero);
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t hilo1;
	pthread_t hilo2;

	params p;
	p.n = 5;

	pthread_create(&hilo1, NULL, duplicar, &p);
	pthread_create(&hilo2, NULL, dividir, &p);

	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	printf("Número: %d\n", numero);
	return 0;
}
