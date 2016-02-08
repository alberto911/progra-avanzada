#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int factorial(int n) {
	int total = 1, i;
	for (i = 2; i <= n; ++i)
		total *= i;
	return total;
}

int main(int argc, const char * argv[]) {
    int pid, estado, numero;
	
	do {
		printf("Introduce un número: ");
		scanf("%d", &numero);

		pid = fork();

		if (pid == -1)
		    printf("Error al crear el proceso hijo \n");
		else if (pid == 0) {
		    printf("%d! = %d\n", numero, factorial(numero));
			exit(0);
		}
		else
			waitpid(pid, &estado, 0);

	} while (numero != 0);
    
    return 0;
}
