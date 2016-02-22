#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L 2

void leerYEscribir(int* fd1, int* fd2);

int main(int argc, const char * argv[])
{
	int n = atoi(argv[1]);   

	int** pipes = (int**) malloc(n * sizeof(int*));
	int** p;
	for (p = pipes; p < pipes + n; ++p) {
		*p = (int*) malloc(2 * sizeof(int));
		pipe(*p);
	}

    int pid, i = 0, ppid = getpid();
	int *leer, *escribir;

	char token = 't';
    write((*pipes)[1], &token, sizeof(char));
   	
    for (i = 0; i < n; ++i) {
		pid = fork();
		
		if (pid == -1) {
		    printf("Error al crear el proceso hijo");
		    return 1;
		}
		else if (pid == 0) {
			leer = *(pipes + i);
			close(leer[1]);
			escribir = *(pipes + (i+1)%n);
			close(escribir[0]);
			break;
		}
	}
		
	if (ppid != getpid()) {
		int counter = 0;
		while (counter < L) {
			leerYEscribir(leer, escribir);
			++counter;
		}
		
		for (p = pipes; p < pipes + n; ++p)
			free(*p);
		free(pipes);
		exit(0);
	}

	int total = n, status;
	while (total > 0) {
		wait(&status);
		--total;
	}

	for (p = pipes; p < pipes + n; ++p)
		free(*p);
	free(pipes);
  
    return 0;
}

void leerYEscribir(int* fd1, int* fd2)
{    
	char c;    
	read(fd1[0], &c, sizeof(char));
	printf("Soy el proceso %d y tendré el testigo %c por 5 segundos\n", getpid(), c);
	sleep(5);

	printf("Soy el proceso %d y acabo de mandar el testigo %c\n", getpid(), c);
	write(fd2[1], &c, sizeof(char));
}
