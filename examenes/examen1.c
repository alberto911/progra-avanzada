#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char c;
	int f;
} frecuencia;

int main() {
	int max;
	printf("Longitud máxima: ");
	scanf("%d", &max);
	
	char* str = (char*) malloc(max * sizeof(char));
	int* ascii = (int*) malloc(256 * sizeof(int));

	int *p, q;

	for (p = ascii; p < ascii + 256; ++p)
		*p = 0;

	getchar();	
	printf("Introduce el string: ");
	fgets(str, max, stdin);

	int len = strlen(str);
	char* c;

	for (c = str; c < str + len; ++c) {
		*(ascii + *c) += 1;
	}
	
	for (p = ascii; p < ascii + 256; ++p) {
		if (*p != 0 && !((int) (p - ascii) == ' ' || (int) (p - ascii) == '\n')) {
			printf("%c  %d  ", (int) (p - ascii), *p);
			for (q = 0; q < *p; ++q)
				printf("*");
			printf("\n");
		}
	}

	free(ascii);
	free(str);
}
