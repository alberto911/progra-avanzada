#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

#define N 5000
#define T 0.01

typedef struct {
	float x;
	float y;
	int c;
} dato;

int main() {
	int i, totalC1, totalC2;
	double dC1, dC2, d1, d2, c1x, c1y, c2x, c2y;
	dato datos[N];
	dato c1, c2, c1Old, c2Old;

	srand((int) time(NULL));
	
	// Llenar datos
	#pragma omp parallel shared(datos) private(i)
	{
		int seed = omp_get_thread_num();
		#pragma omp for
		for (i = 0; i < N; ++i) {
			datos[i].x = ((float)(rand_r(&seed) % 101)) / 100;
			datos[i].y = ((float)(rand_r(&seed) % 101)) / 100;
		}
	}

	// Centroides aleatorios
	int indexC1, indexC2;
	indexC1 = rand() % N;
	do {
		indexC2 = rand() % N;
	} while (indexC2 == indexC1);
	c1 = datos[indexC1];
	c2 = datos[indexC2];

	printf("C1: %f %f\n", c1.x, c1.y);
	printf("C2: %f %f\n", c2.x, c2.y);

	// Obtener distancias
	do {
		c1Old = c1;
		c2Old = c2;
		totalC1 = totalC2 = 0;
		c1x = c1y = c2x = c2y = 0.0;

		#pragma omp parallel for default(shared) private(i,d1,d2) schedule(static) reduction(+:totalC1,totalC2,c1x,c1y,c2x,c2y)
		for (i = 0; i < N; ++i) {
			d1 = sqrt(pow((c1Old.x - datos[i].x), 2) + pow((c1Old.y - datos[i].y), 2));
			d2 = sqrt(pow((c2Old.x - datos[i].x), 2) + pow((c2Old.y - datos[i].y), 2));
			if (d1 < d2) {
				datos[i].c = 1;
				c1x = c1x + datos[i].x;
				c1y = c1y + datos[i].y;
				totalC1 = totalC1 + 1;
			}
			else {
				datos[i].c = 2;
				c2x = c2x + datos[i].x;
				c2y = c2y + datos[i].y;
				totalC2 = totalC2 + 1;
			}
		}
	
		c1.x = c1x / totalC1;
		c1.y = c1y / totalC1;
		c2.x = c2x / totalC2;
		c2.y = c2y / totalC2;

		dC1 = sqrt(pow((c1Old.x - c1.x), 2) + pow((c1Old.y - c1.y), 2));
		dC2 = sqrt(pow((c2Old.x - c2.x), 2) + pow((c2Old.y - c2.y), 2));

		printf("\nC1: %f %f\n", c1.x, c1.y);
		printf("C2: %f %f\n", c2.x, c2.y);	
	} while (dC1 > T || dC2 > T);

	return 0;
}
