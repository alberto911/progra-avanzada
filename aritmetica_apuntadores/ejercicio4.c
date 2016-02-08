#include <stdio.h>
#include <stdlib.h>

int main() {
	int i, n, m;
	printf("Introduce N: ");
	scanf("%d", &n);
	printf("Introduce M: ");
	scanf("%d", &m);

	int** matriz = (int**) malloc(n * sizeof(int*));
	for (i = 0; i < n; ++i)
		*(matriz + i) = (int*) malloc(m * sizeof(int));

	int **row, *col, j;

	for (row = matriz, i = 0; row < matriz + n; row++, ++i) {
		for (col = *row, j = 0; col < *row + m; col++, ++j) {
			printf("Elemento [%d, %d]: ", i, j);
			scanf("%d", col);
		}
	}

	for (row = matriz; row < matriz + n; row++) {
		for (col = *row; col < *row + m; col++)
			printf("%d ", *col);
		printf("\n");
	}

	for (i = 0; i < n; ++i)
		free(*(matriz + i));
	free(matriz);
}
