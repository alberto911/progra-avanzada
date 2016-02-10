#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char** argv) { 
	int n = 1, c, i, pid;
	char* p;

	while ((c = getopt(argc, argv, "n:p:")) != -1) {
		switch (c) {
			case 'n':
				n = atoi(optarg);
				break;
			case 'p':
				p = optarg;
				break;
			default:
				return 1;
		}
	}

	for (i = 0; i < n; ++i) {
		pid = fork();
		if (pid == -1) {
			printf("Error al crear el proceso\n");
			return 2;
		}
		else if (pid == 0) {
			execlp(p, p, (char*) NULL);
			exit(0);
		}
	}

	int estado, id;
	while (n > 0) {
		id = wait(&estado);
		printf("Acabó el proceso hijo con PID %d.\n", id);
		--n;
	}
	
    return 0;
}
