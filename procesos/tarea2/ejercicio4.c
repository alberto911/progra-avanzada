#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int crearProcesos(int, int, int);

int main(int argc, char** argv) { 
	int n = 1, p = 1, c;

	while ((c = getopt(argc, argv, "n:p:")) != -1) {
		switch (c) {
			case 'n':
				n = atoi(optarg);
				break;
			case 'p':
				p = atoi(optarg);
				break;
			default:
				return 1;
		}
	}

	crearProcesos(n, p, 0);
	
    return 0;
}

int crearProcesos(int n, int p, int counter) {
	if (counter++ < n) { 	
		int *i, estado;
		int* pids = (int*) malloc(p * sizeof(int)); 

		for (i = pids; i < pids + p; ++i) {
			*i = fork();
			if (*i == -1) {
				printf("Error al crear el proceso\n");
				return 2;
			}
			else if (*i == 0) {
				crearProcesos(n, p, counter);
				exit(0);
			}
		}

		// Esperar a que acaben los hijos
		int hijos = p;
		while (hijos > 0) {
			wait(&estado);
			--hijos;
		}

		// Imprimir
		for (hijos = 1; hijos < counter; ++hijos)
			printf("    ");
		printf("%d ----> ", getpid());

		for (i = pids; i < pids + p; ++i)
			printf("%d ", *i);
		printf("\n");

		free(pids);
	}
}
