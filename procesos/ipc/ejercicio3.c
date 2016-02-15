#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int factorial(int);
void leer(int* fd1, int* fd2);
void escribir(int* fd1, int* fd2);

int main(int argc, const char * argv[])
{
    int tuberia[2];
	int imprimir[2];
    pid_t pid;
    
    pipe(tuberia);
	pipe(imprimir);
    
    pid = fork();
    
    if (pid == -1)
    {
        printf("Error al crear el proceso hijo");
        return 1;
    }
    else if (pid == 0)
    {
        leer(tuberia, imprimir);
    }
    else {
        escribir(tuberia, imprimir);
    }
    
    return 0;
}

int factorial(int n) {
	int total = 1, i;
	for (i = 2; i <= n; ++i)
		total *= i;
	return total;
}

void leer(int* fd1, int* fd2)
{
    int n, fact;
    
    while (1)
    {
		close(fd1[1]);
		read(fd1[0], &n, sizeof(int));
		if (n == 0)
			exit(0);
        
		close(fd2[0]);
		fact = factorial(n);
		write(fd2[1], &fact, sizeof(int));
    }
}

void escribir(int* fd1, int* fd2)
{
    int n = -1, fact;

    while (1) {
        printf("Introduce un número: ");
		scanf("%d", &n);
		close(fd1[0]);
        write(fd1[1], &n, sizeof(int));

		if (n == 0)
			break;

		close(fd2[1]);
		read(fd2[0], &fact, sizeof(int));
		printf("%d! = %d\n", n, fact);
    }
}
