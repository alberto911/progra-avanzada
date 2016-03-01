#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int time = 3, ctrlC = 0, ctrlZ = 0;
void terminar(int);
void manejador(int);
void alarma(int);

int main(int argc, const char * argv[])
{
    int pid;
	pid = fork();

	if (pid == -1) {
		printf("Error al crear el proceso\n");
		return 1;
	}
	else if (pid == 0) {
		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		sleep(10);
		kill(getppid(), SIGUSR1);
	}
	else {
		signal(SIGUSR1, terminar);
		signal(SIGINT, manejador);
		signal(SIGTSTP, manejador);
		signal(SIGALRM, alarma);

		while (1) {
			alarm(time);
			pause();
		}
	}

	return 0;
}

void alarma(int s) {
	printf("Aparezco cada %d segundos\n", time);
}

void manejador(int s) {
	if (s == SIGINT) {
		++time;
		ctrlC++;
	}
	else {
		ctrlZ++;
		if (time > 1)
			--time;
	}
	pause();
}

void terminar(int s) {
	printf("Se ha pulsado %d veces Ctrl+C y %d veces Ctrl+Z. Terminando...\n", ctrlC, ctrlZ);
	kill(SIGKILL, 0);
	raise(SIGKILL);
}
