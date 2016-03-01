#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define N 5 // Archivos a crear
#define T 3 // Tiempo

int grabar;
void crearDirectorio();
void temporizador(int);

int main(int argc, const char * argv[])
{
	signal(SIGALRM, temporizador);
    crearDirectorio();

	int i, fd, len = ((N-1) / 10) + 9;
	char* buffer = (char*) malloc(len * sizeof(char));
	struct stat st;
	
	for (i = 0; i < N; ++i) {
		grabar = 1;
		snprintf(buffer, len, "datos/a%d", i); // Nombre del archivo
		fd = open(buffer, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		
		// Escribir en el archivo
		alarm(T);
		while (grabar)
			write(fd, "x", sizeof(char));

		// Imprimir tamaño
		fstat(fd, &st);
		printf("a%d   %d\n", i, (int)st.st_size);		

		close(fd);
	}

	free(buffer);
	return 0;
}

void temporizador(int s) {
	grabar = 0;
}

void crearDirectorio() {
	DIR* dir = opendir("datos");
	if (dir) {
		system("rm -R datos");
		closedir(dir);
	}
	system("mkdir datos");
}
