// Tiempo de ejecución: 4.179 s
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define N 10000
#define M 100

int digitos(int num) {
	int counter = 0;
	while (num >= 1) {
		counter++;
		num /= 10;
	}
	return counter;
}

int esPrimoCircular(int num) {
	int len = digitos(num), i, primoCircular = 1;
	if (!esPrimo(num))
		return 0;
	for (i = 1; i < len; ++i) {
		int new = (pow(10, len-i) * (num % (int) pow(10, i))) + floor(num / pow(10, i));
		if (!esPrimo(new))
			return 0;
	}
	return 1;
}

int esPrimo(int num) {
	int i;
	if (num < 2)
		return 0;
	for (i = 2; i <= sqrt(num); ++i)
		if (num % i == 0)
			return 0;
	return 1;
}

int main() {
	int i, j, k, total = 0;
	int* primosCirculares = NULL;

	// Guardar todos los primos circulares en el rango
	for (i = M; i <= N; ++i) {
		if (esPrimoCircular(i)) {
			primosCirculares = (int*) realloc(primosCirculares, ++total * sizeof(int));
			primosCirculares[total-1] = i;
		}
	}

	short** numeros = (short**) malloc(N * sizeof(short*));
	short** esPC = (short**) malloc(N * sizeof(short*));
	int mod = N - M + 1;

	for (i = 0; i < N; ++i) {
		numeros[i] = (short*) malloc(N * sizeof(short));
		esPC[i] = (short*) malloc(N * sizeof(short));
	}

	#pragma omp parallel shared(numeros,esPC,mod) private(i,j)
	{
		// Inicializar matrices
		int seed = omp_get_thread_num();
		#pragma omp for schedule(auto)
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				numeros[i][j] = (rand_r(&seed) % mod) + M;
				esPC[i][j] = 0;
			}
		}
	}

	#pragma omp parallel shared(numeros,esPC,primosCirculares,total) private(i,j,k)
	{
		// Checar si son primos
		#pragma omp for schedule(auto)
		for (i = 0; i < N; ++i) {
			for (j = 0; j < N; ++j) {
				for (k = 0; k < total; ++k) {
					if (numeros[i][j] == primosCirculares[k]) {
						esPC[i][j] = 1;
						break;				
					}
				}
			}
		}
	}

	// Liberar memoria
	for (i = 0; i < N; ++i) {
		free(numeros[i]);
		free(esPC[i]);
	}
	free(primosCirculares);	
	free(numeros);
	free(esPC);
	return 0;
}
