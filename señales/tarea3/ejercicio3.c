#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int prisa = 0;
void alarma(int);

int main(int argc, const char * argv[]) {
	if (argc < 2 || atoi(argv[1]) < 5 || atoi(argv[1]) > 9) {
		printf("Debe haber un parámetro entre 5 y 9\n");
		return 1;
	}

	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, alarma);

	int *array = (int*) malloc(atoi(argv[1]) * sizeof(int)), *p;
	
	alarm(3);

	for (p = array; p < array + atoi(argv[1]); ++p) {
		if (prisa)
			printf("Dese prisa. ");
		printf("Dato %d: ", (int) (p - array));
		scanf("%d", p);
	}

	printf("\nDatos:\n");
	for (p = array; p < array + atoi(argv[1]); ++p) {
		printf("%d\n", *p);
	}

	free(array);
	return 0;
}

void alarma(int s) {
	prisa = 1;
}
