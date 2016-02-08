#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, const char * argv[]) { 
	int hijos, i, pid;
	int ppid = getpid();

	printf("Número de procesos hijos: ");
	scanf("%d", &hijos);

	int* pids = (int*) malloc(hijos * sizeof(int));

	for (i = 0; i < hijos; ++i) {
		pids[i] = fork();

		if (pids[i] == -1) {
		    printf("Error al crear el proceso hijo \n");
			return 1;
		}
		else if (pids[i] == 0)
			break;
	}

	pid = getpid();
	if (pid != ppid) {
		srand(pid);
		int random = rand() % 10 + 1;
		sleep(random);
		exit(0);
	}
	else {
		int estado;
		while (hijos > 0) {
			pid = wait(&estado);
			printf("Terminó el proceso hijo con PID %d\n", pid);
			--hijos;
		}
	}
    
	free(pids);
    return 0;
}
