#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

typedef void* (*pos_t)(void*, int);
typedef void* (*next_t)(void*);
typedef void (*print_t)(void*);
typedef void (*iterator_t)(void*, pos_t, pos_t, next_t, next_t, print_t, size_t, int);

//void recorre(void* a, iterator_t it);
void forwardIterator(void* a, next_t begin, pos_t end, next_t next, next_t prev, print_t print, size_t size, int total);
void reverseIterator(void* a, next_t begin, pos_t end, next_t next, next_t prev, print_t print, size_t size, int total);
void* beginInt(void* p);
void* endInt(void* p, int total);
void* nextInt(void* p);
void* prevInt(void* p);
void printInt(void* p);

int main() {
	int* arreglo = (int*) malloc(N * sizeof(int)), *p;

	srand((int) time(NULL));
    
    for (p = arreglo; p < arreglo + N; ++p)
        *p = rand() % 100;

	forwardIterator(arreglo, beginInt, endInt, nextInt, prevInt, printInt, sizeof(int), N);
	printf("\n");
	reverseIterator(arreglo, beginInt, endInt, nextInt, prevInt, printInt, sizeof(int), N);
	free(arreglo);
}

/*void recorre(void* a, iterator_t it) {
	it(a);
}*/

void forwardIterator(void* a, next_t begin, pos_t end, next_t next, next_t prev, print_t print, size_t size, int total) {
	void* inicio = begin(a);
	void* fin = end(a, total);
	while (inicio <= fin) {
		print(inicio);
		inicio = next(inicio);
	} 
}

void reverseIterator(void* a, next_t begin, pos_t end, next_t next, next_t prev, print_t print, size_t size, int total) {
	void* inicio = end(a, total);
	void* fin = begin(a);
	while (inicio >= fin) {
		print(inicio);
		inicio = prev(inicio);
	} 
}

void* beginInt(void* p) {
	return p;
}

void* endInt(void* p, int total) {
	return (int*) p + total - 1;
}

void* nextInt(void* p) {
	return (int*) p + 1;
}

void* prevInt(void* p) {
	return (int*) p - 1;
}

void printInt(void* p) {
	printf("%d ", *(int*) p);
}
