#include <stdio.h>
#include <omp.h>

int main ()  {

int   i, n, chunk;
float a[100], b[100], result, r2;

/* Some initializations */
n = 100;
chunk = 10;
result = 0.0;
r2 = 1;
for (i=0; i < n; i++)
  {
  a[i] = i * 1.0;
  b[i] = i * 2.0;
  }

#pragma omp parallel for default(shared) private(i) schedule(static,chunk) reduction(+:result,r2)
 for (i=0; i < n; i++) {
    result = result + (a[i] * b[i]);
	r2 = r2 + (a[i] * b[i]);
}

printf("Final result= %f %f\n",result,r2);

}

