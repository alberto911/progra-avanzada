#include <omp.h>
#include <stdio.h>

#define n 100

int main()
{
    int i;
    float a[n],b[n],c[n],d[n];
    for (i = 0; i < n; i++) {
        a[i]=i*1.5;
        b[i]=i+22.32;
    }
    
    #pragma omp parallel shared(a,b,c,d) private(i)
    {
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                for (i = 0; i < n; i++) {
                    c[i]=a[i]+b[i];
                }
            }
            #pragma omp section
            {
                for (i = 0; i < n; i++) {
                    d[i]=a[i]*b[i];
                }
            }
        }

		#pragma omp single
        {
            printf("Número de threads: %d\n", omp_get_num_threads());
        }
    }

	for (i = 0; i < n; ++i)
		printf("%f ", c[i]);
	printf("\n\n");

	for (i = 0; i < n; ++i)
		printf("%f ", d[i]);
	printf("\n\n");
    
    return 0;
}
