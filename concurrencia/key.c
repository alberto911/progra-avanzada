#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_key_t key;

void key_destruct(void* value)
{
    free(value);
    pthread_setspecific(key, NULL);
}

void setString() {
	char* string = (char*) pthread_getspecific(key);
	if (string == NULL) {
		string = (char*) malloc(10 * sizeof(char));
		sprintf(string, "Hola!");
		pthread_setspecific(key, (void*) string);
	}
}

void setNumber() {
	int* number = (int*) pthread_getspecific(key);
	if (number == NULL) {
		number = (int*) malloc(sizeof(int));
		*number = 3;
		pthread_setspecific(key, (int*) number);
	}
}

void* function1(void *arg) {
    int i;
	setString();
	for (i = 0; i < 5; ++i) {
		printf("%s\n", (char*) pthread_getspecific(key));
		sleep(1);
	}
}

void* function2(void *arg) {
    int i;
	setNumber();
	for (i = 0; i < 5; ++i) {
		printf("%d\n", *((int*) pthread_getspecific(key)));
		sleep(1);
	}
}

int main( void )
{
    pthread_key_create(&key, key_destruct);
	pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, &function1, NULL);
	pthread_create(&thread2, NULL, &function2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
    
    return 0;
}
