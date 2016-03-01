#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 4

void manejador(int s) {
	
}

int main(int argc, const char * argv[]) {
	int *pids = (int*) malloc(N * sizeof(int)), *p;
	int ppid = getpid();

	for (p = pids; p < pids + N; ++p) {
		*p = fork();

		if (*p == -1) {
			printf("Error al crear el proceso\n");
			return 1;
		}
		else if (*p == 0) {
			signal(SIGUSR1, manejador);
			int verde = 0;		
			break;
		}
		else if (p == pids)
			kill(*p, SIGUSR1);
	}

	free (pids);
	return 0;
}
