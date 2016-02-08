#include <stdio.h>
#include <stdlib.h>

#define N 20

/* Tipos de funciones */
typedef int (*t_compare)(void *, void *);
typedef void (*t_swap)(void * , void * );
typedef void (*t_sort)(void *, size_t, size_t, t_compare, t_swap);
typedef void (*t_recursiveSort)(void*, int, int, int, size_t, t_compare, t_swap);

/* Funciones genéricas */
void sort(t_sort, void *, size_t , size_t, t_compare, t_swap);
void recursiveSort(t_recursiveSort, void*, int, int, int, size_t, t_compare, t_swap);

void insertionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap);
void selectionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap);
void quicksort(void* v, int primero, int ultimo, int total, size_t size, t_compare compara, t_swap swap);
void mergesort(void* v, int l, int n, int total, size_t size, t_compare comp, t_swap assign);
void merge(void* v, int l, int m, int n, int total, size_t size, t_compare comp, t_swap assign);

/* Funciones callback para enteros */
int ascInt(void* , void*);
int descInt(void* , void*);

void swapInt(void* , void*);
void assignInt(void*, void*);

/* Funciones callback para flotantes */
int ascFloat(void*, void*);
int descFloat(void*, void*);

void swapFloat(void*, void*);
void assignFloat(void*, void*);

/* Funciones de impresión */
void imprimeInt(int *, int);
void imprimeFloat(float *, int);

int main()
{
	t_sort* sortArray = (t_sort*) malloc(2 * sizeof(t_sort));
	t_recursiveSort* recursiveSortArray = (t_recursiveSort*) malloc(2 * sizeof(t_recursiveSort));
	t_compare* compareArray = (t_compare*) malloc(4 * sizeof(t_compare));
	t_swap* swapArray = (t_swap*) malloc(4 * sizeof(t_swap));

	*sortArray = insertionSort;
	*(sortArray + 1) = selectionSort;

	*recursiveSortArray = quicksort;
 	*(recursiveSortArray + 1) = mergesort;

	*compareArray = ascInt;
	*(compareArray + 1) = descInt;
	*(compareArray + 2) = ascFloat;
	*(compareArray + 3) = descFloat;

	*swapArray = swapInt;
	*(swapArray + 1) = assignInt;
	*(swapArray + 2) = swapFloat;
	*(swapArray + 3) = assignFloat;

	int datatype, recursive, algorithm, order;
	printf("Tipo de dato:\n0. Enteros\n1. Flotantes\nSelecciona: ");
	scanf("%d", &datatype);
	printf("Tipo de algoritmo:\n0. No recursivo\n1. Recursivo\nSelecciona: ");
	scanf("%d", &recursive);
	printf("Algoritmo:");
	if (!recursive) {
		printf("\n0. Insertion\n1. Selection\nSelecciona: ");
		scanf("%d", &algorithm);
	}
	else {
		printf("\n0. Quicksort\n1. Mergesort\nSelecciona: ");
		scanf("%d", &algorithm);
	}
	printf("Tipo de ordenamiento:\n0. Ascendente\n1. Descendente\nSelecciona: ");
	scanf("%d", &order);

	if (datatype == 0) {
		int* vector = (int*) malloc(N * sizeof(int));
		int* aux;
		for (aux = vector; aux < vector + N; ++aux)
		    *aux = rand() % 100;
		
		printf("\n\n--- Enteros ---\n\n");
		imprimeInt(vector, N);

		if (!recursive)
			sort(sortArray[algorithm], vector, N, sizeof(*vector), compareArray[order], swapArray[0]);
		else
			recursiveSort(recursiveSortArray[algorithm], vector, 0, N-1, N, sizeof(*vector), compareArray[order], swapArray[algorithm]);

		printf("\n\n--- Enteros Ordenados ---\n\n");
    	imprimeInt(vector, N);
		free(vector);
	}
	else if (datatype == 1) {
		float* vector = (float*) malloc(N * sizeof(float));
		float* aux;
		for (aux = vector; aux < vector + N; ++aux)
		    *aux = rand() % 100 * 1.5;
		
		printf("\n\n--- Flotantes ---\n\n");
		imprimeFloat(vector, N);

		if (!recursive)
			sort(sortArray[algorithm], vector, N, sizeof(*vector), compareArray[order+2], swapArray[2]);
		else
			recursiveSort(recursiveSortArray[algorithm], vector, 0, N-1, N, sizeof(*vector), compareArray[order+2], swapArray[algorithm+2]);

		printf("\n\n--- Flotantes Ordenados ---\n\n");
    	imprimeFloat(vector, N);
		free(vector);
	}
    
	free(sortArray);
	free(recursiveSortArray);
	free(compareArray);
	free(swapArray);
    return 0;
}

/* Implementación de funciones genéricas */

void sort(t_sort algorithm, void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    (*algorithm)(vector, count, size, compare, swap);
}

void recursiveSort(t_recursiveSort algorithm, void* v, int s, int f, int n, size_t size, t_compare comp, t_swap swap) {
	algorithm(v, s, f, n, size, comp, swap);
}

void insertionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    void * j;
    void * start = vector;
    void * aux = start;
    void * last = start + (count * size);
    
    for (; aux < last ; aux+=size)
    {
        j = aux - size;
        while (j >= start && (*compare)(j, j+size) )
        {
            (*swap)(j+size, j);
            j-=size;
        }
    }
}

void selectionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    void * minimo;
    
    void * j;
    void * start = vector;
    void * aux = start;
    void * last = start + (count * size);
    void * prelast = last - size;
    
    for(; aux < prelast; aux+=size)
    {
        minimo = aux;
        for(j = aux + size; j < last; j+=size)
        {
            if ( (*compare)(minimo, j) )
                minimo = j;
        }
        
        (*swap)(minimo, aux);
    }
}

void mergesort(void* v, int l, int n, int total, size_t size, t_compare comp, t_swap assign)
{
    int m = (n+l)/2;
    if (n > l)
    {
        mergesort (v, l, m, total, size, comp, assign);
        mergesort (v, m+1, n, total, size, comp, assign);
        merge (v, l, m, n, total, size, comp, assign);
    }
}

void merge(void* v, int l, int m, int n, int total, size_t size, t_compare comp, t_swap assign)
{
    int i, j, k;
    void* aux = malloc(total * size);
    
    for (i = m+1; i > l; i--)
        assign(aux + size*(i-1), v + size*(i-1));
    
    for (j = m; j < n; j++)
        assign(aux + size*(n+m-j), v + size*(j+1));
    
    for (k = l; k <= n; k++)
        if (!comp(aux + size*i, aux + size*j))
            assign(v + size*k, aux + size*i++);
        else
            assign(v + size*k, aux + size*j--);

	free(aux);
}

void quicksort(void* v, int primero, int ultimo, int total, size_t size, t_compare compara, t_swap swap)
{
    int izquierdo = primero;
    int derecho = ultimo;
    
    if (primero < ultimo) // Paso base
    {
        // particion
        while (izquierdo < derecho)
        {
            while ((izquierdo < derecho) && !compara(v + izquierdo*size, v + primero*size)) izquierdo++; // <=
            while (compara(v + derecho*size, v + primero*size) ) derecho--; // >
            
            if (izquierdo < derecho) // se intercambian los contenidos
            {
                swap(v + derecho*size, v + izquierdo*size);
            }
        }
        
        // Se intercambia el pivote con el elemento de la posición derecha
        swap(v + derecho*size, v + primero*size);
        
        // Paso recursivo
        quicksort (v, primero, derecho-1, total, size, compara, swap);
        quicksort (v, izquierdo, ultimo, total, size, compara, swap);
    }
}

/* Implementación de funciones callback para enteros */

int ascInt(void * pa, void * pb)
{
    int * a = (int *)pa;
    int * b = (int *)pb;
    return (*a > *b);
}

int descInt(void * pa, void * pb)
{
    int * a = (int *)pa;
    int * b = (int *)pb;
    return (*a < *b);
}

void swapInt(void * pa , void * pb )
{
    int temp;
    int * a = (int *)pa;
    int * b = (int *)pb;
    
    temp = *a;
    *a = *b;
    *b = temp;
    
}

void assignInt(void* a, void* b) {
	*(int*) a = *(int*) b;
}

/* Implementación de funciones callback para flotantes */

int ascFloat(void * pa, void * pb)
{
    float * a = (float *)pa;
    float * b = (float *)pb;
    return (*a > *b);
}

int descFloat(void * pa, void * pb)
{
    float * a = (float *)pa;
    float * b = (float *)pb;
    return (*a < *b);
}

void swapFloat(void * pa , void * pb )
{
    float temp;
    float * a = (float *)pa;
    float * b = (float *)pb;
    
    temp = *a;
    *a = *b;
    *b = temp;
}

void assignFloat(void* a, void* b) {
	*(float*) a = *(float*) b;
}

/* Implementación de funciones de impresión */

void imprimeInt(int * vector, int count)
{
    int * aux = vector;
    int * last = vector + count;
    
    for (; aux < last; ++aux) {
        printf(" %4d ", *aux);
    }
    
    printf("\n");
}

void imprimeFloat(float * vector, int count)
{
    float * aux = vector;
    float * last = vector + count;
    
    for (; aux < last; ++aux) {
        printf(" %7.2f ", *aux);
    }
    
    printf("\n");
}
