#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
	int i, j;
	int numeros[6];
	int seeds[6];

	srand((int) time(NULL));
	for (j = 0; j < 6; ++j)
		seeds[j] = rand();

	omp_set_nested(1);
	#pragma omp parallel num_threads(2)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				#pragma omp critical
				{
					#pragma omp parallel shared(i) num_threads(4)
					{
						printf("Thread %d de %d\n", omp_get_thread_num(), omp_get_num_threads());
						#pragma omp atomic
						i++;
					}
				}
			}

			#pragma omp section
			{
				#pragma omp parallel shared(numeros, seeds) num_threads(6)
				{
					int seed = seeds[omp_get_thread_num()];
					numeros[omp_get_thread_num()] = rand_r(&seed) % 10000;
					#pragma omp barrier

					#pragma omp master
					{
						printf("\nIteraciones: %d\n", omp_get_num_threads());
						int sum = 0;
						for (j = 0; j < 6; ++j)
							sum += numeros[j];
						printf("Suma: %d\n", sum);
						printf("Promedio: %f\n", (sum + 0.0) / 6);
					}
				}
			}
		}
	}

	return 0;
}
