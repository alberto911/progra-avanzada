#include<omp.h>
#include<stdio.h>

int main()
{
    int i, n=100, chunk=50, tid;
    float a[n], b[n], c[n];
    
    for (i = 0; i < n; i++)
        a[i]=b[i]=i*22.0;
    //chunk=CHUNK;
    
    #pragma omp parallel shared(a,b,c,chunk) private(i,tid)
    {
        #pragma omp for schedule(auto)
        for(i=0;i<n;i++)
        {
            tid = omp_get_thread_num();
            c[i]=a[i] + b[i];
            printf("tid %d i %d\n",tid,i);
        }
    }

	for (i = 0; i < n; ++i)
		printf("%.0f ", c[i]);
    
    return 0;
}
