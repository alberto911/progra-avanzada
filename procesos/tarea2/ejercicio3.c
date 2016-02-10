#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, const char * argv[]) { 
	int hijos, *i, pid, estado;

	printf("Número de procesos hijos: ");
	scanf("%d", &hijos);

	int* pids = (int*) malloc(hijos * sizeof(int));

	for (i = pids; i < pids + hijos; ++i) {
		*i = fork();

		if (*i == -1) {
		    printf("Error al crear el proceso hijo \n");
			return 1;
		}
		else if (*i == 0) {
			srand(getpid());
			int random = rand() % 10 + 1;
			sleep(random);
			exit(0);
		}
	}

	while (hijos > 0) {
		pid = wait(&estado);
		printf("Terminó el proceso hijo con PID %d\n", pid);
		--hijos;
	}
    
	free(pids);
    return 0;
}
