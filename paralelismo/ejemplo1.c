#include<omp.h>
#include<stdio.h>
int main() {
    int nthreads, tid;
    #pragma omp parallel private(tid)
	{
        tid = omp_get_thread_num();
        printf("\nprueba");
        if(tid == 0) { // Master thread
            nthreads=omp_get_num_threads();
            printf("\nNT: %d",nthreads);
        }
    }
    return 0;
}
