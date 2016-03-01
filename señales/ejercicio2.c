#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void manejador(int s) {
	printf("Este proceso no puede ser detenido!\nPulsaste %d\n", s);
}

int main(int argc, const char * argv[])
{
    int pid;

	if (signal(SIGTSTP, manejador) == SIG_ERR)
        printf("ERROR: No se pudo llamar al manejador\n");

	pid = fork();

	if (pid == -1) {
		printf("Error al crear el proceso\n");
		return 1;
	}
	else if (pid == 0) {
		if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        	printf("ERROR: No se pudo ignorar\n");

		sleep(2);
		printf("PID: %d, PPID: %d\n", getpid(), getppid());

		int i;
		for (i = 10; i > 0; --i) {
			printf("%d\n", i);
			sleep(2);
		}
	
		printf("PID: %d, PPID: %d\n", getpid(), getppid());
		exit(0);
	}
	else {
		printf("PID: %d, Si se pulsa Ctrl+C solo morirá el padre\n", getpid());
		sleep(10);
		wait(NULL);
	}

	return 0;
}
