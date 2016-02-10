#include <stdio.h>
#include <unistd.h>

int factorial(int);
void leer(int fd);
void escribir(int fd);

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
        close(tuberia[1]);
        leer(tuberia[0]);
    }
    else {
        close(tuberia[0]);
        escribir(tuberia[1]);
    }
    
    return 0;
}

int factorial(int n) {
	int total = 1, i;
	for (i = 2; i <= n; ++i)
		total *= i;
	return total;
}

void leer(int fd)
{
    FILE * file;
    int n;
    
    file = fdopen(fd, "r");
    
    while (fscanf(file, "%d", &n) != EOF)
    {
        printf("%d! = %d\n", n, factorial(n));
    }
    
    fclose(file);
}

void escribir(int fd)
{
    FILE * file;
    file = fdopen(fd, "w");
    
    int n;
    do {
        printf("Introduce un número: ");
		scanf("%d", &n);
        fprintf(file, "%d\n", n);
    } while (n != 0);
    
    fclose(file);
}
