#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

typedef int (*comp_t)(void*, void*);
typedef void (*swap_t)(void*, void*);

typedef struct() {
	char* titulo;
	int* paginas;
} libro;

int asc_int(void*, void*);
int desc_int(void*, void*);
int asc_float(void*, void*);
int desc_float(void*, void*);

void swap_int(void*, void*);
void swap_float(void*, void*);

void bubbleSort(void*, int, size_t, comp_t, swap_t);
void insertionSort(int*, int, comp_t);
void selectionSort(int*, int);

int main() {
	// Integers
	int* enteros = (int*) malloc(N * sizeof(int));
	int *p;

	srand((int) time(NULL));
    
    for (p = enteros; p < enteros + N; ++p) {
        *p = rand() % 100;
        printf("%d ", *p);
    }
	printf("\n");

	bubbleSort(enteros, N, sizeof(int), asc_int, swap_int);

	for (p = enteros; p < enteros + N; ++p) {
        printf("%d ", *p);
    }
	printf("\n");

	free(enteros);

	// Floats
	float* reales = (float*) malloc(N * sizeof(float));
	float *q;

	srand((int) time(NULL));
    
    for (q = reales; q < reales + N; ++q) {
        *q = rand() % 100 + 0.5;
        printf("%f ", *q);
    }
	printf("\n");

	bubbleSort(reales, N, sizeof(float), desc_float, swap_float);

	for (q = reales; q < reales + N; ++q) {
        printf("%f ", *q);
    }
	printf("\n");

	free(reales);
	return 0;
}

int asc_int(void* a, void* b) {
	return (*(int*) a > *(int*) b);
}

int desc_int(void* a, void* b) {
	return (*(int*) a < *(int*) b);
}

int asc_float(void* a, void* b) {
	return (*(float*) a > *(float*) b);
}

int desc_float(void* a, void* b) {
	return (*(float*) a < *(float*) b);
}

void swap_int(void* a, void* b) {
	int temp = *(int*) a;
	*(int*) a = *(int*) b;
	*(int*) b = temp;
}

void swap_float(void* a, void* b) {
	float temp = *(float*) a;
	*(float*) a = *(float*) b;
	*(float*) b = temp;
}

void bubbleSort(void* a, int n, size_t size, comp_t comp, swap_t swap) {
	int swapped = 1;
	void *i; 
	void *end = a + (n * size);

	while (swapped) {
		swapped = 0;
		end -= size;
		for (i = a; i < end; i += size) {
			if (comp(i, i+size)) {
				swap(i, i+size);
				swapped = 1;
			}
		}
	}
}

void insertionSort(int* a, int n, comp_t comp) {
	int i, j;
	for (i = 1; i < n; ++i) {
		for (j = i; j > 0 && comp(a+j-1, a+j); --j) {
			int temp = a[j];
			a[j] = a[j - 1];
			a[j - 1] = temp;
		}
	}
}

void selectionSort(int* a, int n) {
	int i, j;
	for (i = 0; i < n-1; ++i) {
		int min = i;
		for (j = i+1; j < n; ++j) {
			if (a[j] < a[min])
				min = j;
		}
		
		if (min != i) {
			int temp = a[i];
			a[i] = a[min];
			a[min] = temp;
		}
	}
}

