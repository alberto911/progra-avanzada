#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, const char * argv[]) { 
	int hijos, *i, pid, counter = 0;

	printf("Número de procesos hijos: ");
	scanf("%d", &hijos);

	int* pids = (int*) malloc(hijos * sizeof(int));

	for (i = pids; i < pids + hijos; ++i, ++counter) {
		*i = fork();

		if (*i == -1) {
		    printf("Error al crear el proceso hijo. Se han creado %d procesos.\n", counter);
			return 1;
		}
		else if (*i == 0) {
			printf("Promedio: %.1f\n", (getpid() + getppid() + 0.0) / 2);
			exit(0);
		}
	}

	int estado;
	for (i = pids; i < pids + hijos; ++i) {
		waitpid(*i, &estado, 0);
	}
	printf("Han terminado todos los procesos hijos\n");
    
	free(pids);
    return 0;
}
