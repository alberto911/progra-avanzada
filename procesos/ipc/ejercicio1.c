#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int factorial(int);
void leer(int* fd);
void escribir(int* fd);

int main(int argc, const char * argv[])
{
    int tuberia[2];
    pid_t pid;
    
    pipe(tuberia);
    
    pid = fork();
    
    if (pid == -1)
    {
        printf("Error al crear el proceso hijo");
        return 1;
    }
    else if (pid == 0)
    {
        leer(tuberia);
    }
    else {
        escribir(tuberia);
    }
    
    return 0;
}

int factorial(int n) {
	int total = 1, i;
	for (i = 2; i <= n; ++i)
		total *= i;
	return total;
}

void leer(int* fd)
{
    int n;
    
    while (1)
    {
		close(fd[1]);
		read(fd[0], &n, sizeof(int));
		if (n == 0)
			exit(0);
        printf("%d! = %d\n", n, factorial(n));
    }
}

void escribir(int* fd)
{
    int n = -1;

    while (n != 0) {
        //printf("Introduce un número: ");
		scanf("%d", &n);
		close(fd[0]);
        write(fd[1], &n, sizeof(int));
    }
}
