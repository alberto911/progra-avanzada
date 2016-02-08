#include <stdio.h>
#include <stdlib.h>

int main() {
	char* str = NULL;
	size_t length = 0;
	getline(&str, &length, stdin);

	int i = 0;
	while (*(str + i) != '\n') {
		i++;
	}

	printf("Longitud: %d\n", i);
	free(str);
}
