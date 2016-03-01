#include <signal.h>
#include <stdio.h>

void manejador1(int s) {
	printf("Estoy aprendiendo a manejar señales\n");
}

void manejador2(int s) {
	printf("Ahora cambié de manejador\n");
}

int main(int argc, const char * argv[])
{
	int i;
	void (*gestor)(int);

	if (signal(SIGINT, manejador1) == SIG_ERR)
        printf("ERROR: No se pudo llamar al manejador\n");

	for (i = 1; i <= 10; ++i) {
		printf("Ovejita %d\n", i);
		sleep(1);
	}

	gestor = signal(SIGINT, manejador2);
	for (i = 1; i <= 10; ++i) {
		printf("Trineo %d\n", i);
		sleep(1);
	}

	signal(SIGINT, gestor);
	for (i = 1; i <= 10; ++i) {
		printf("Cabrita %d\n", i);
		sleep(1);
	}

	return 0;
}
