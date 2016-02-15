#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void leerYEscribir(int* fd1, int* fd2);

int main(int argc, const char * argv[])
{
	int n = atoi(argv[1]);
    
	int **pipes = (int**) malloc(n * sizeof(int*)), **p;
	for (p = pipes; p < pipes + n; ++p) {
		*pipes = (int*) malloc(2 * sizeof(int));
		pipe(*pipes);
	}

    int *pids = (int*) malloc(n * sizeof(int)), *pid, i = 0;

	char token = 't';
    write((*(pipes))[1], &token, sizeof(char));
   	
    for (pid = pids; pid < pids + n; ++pid) {	
		*pid = fork();
		
		if (*pid == -1)
		{
		    printf("Error al crear el proceso hijo");
		    return 1;
		}
		else if (*pid == 0)
		{
			int j = i % n;
			i = (i+1) % n;
			leerYEscribir(*(pipes + j), *(pipes + i));
			exit(0);
		}
	}
	
	int total = n, status;
	while(total > 0) {
		wait(&status);
		total--;
	}

	for (p = pipes; p < pipes + n; ++p)
		free(*p);
	free(pipes);
	free(pids);
  
    return 0;
}

void leerYEscribir(int* fd1, int* fd2)
{    
	char c;    
	close(fd1[1]);
	read(fd1[0], &c, sizeof(char));
	printf("Soy el proceso %d y tendré el testigo %c por 5 segundos\n", getpid(), c);
	sleep(5);

	printf("Soy el proceso %d y acabo de mandar el testigo %c\n", getpid(), c);
	close(fd2[0]);
	write(fd2[1], &c, sizeof(char));
}
