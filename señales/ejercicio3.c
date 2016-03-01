#include <signal.h>
#include <stdio.h>

void manejador(int s) {
	if (s == SIGTSTP)
		printf("Pulsaste Ctrl+Z\n");
	else
		printf("Pulsaste Ctrl+C");
}

int main(int argc, const char * argv[])
{
	if (signal(SIGTSTP, manejador) == SIG_ERR || signal(SIGINT, manejador) == SIG_ERR)
        printf("ERROR: No se pudo llamar al manejador\n");

	int i;
	for (i = 10; i > 0; --i) {
		printf("%d\n", i);
		sleep(2);
	}

	return 0;
}
