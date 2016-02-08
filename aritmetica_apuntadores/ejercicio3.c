#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char* str = NULL;
	size_t length = 0;
	getline(&str, &length, stdin);

	int i, len = strlen(str) - 1;
	char* copia = (char*) malloc(len * sizeof(char));
	
	for (i = 0; i < len; ++i) {
		*(copia + i) = *(str + i);
	}

	printf("%s\n", copia);
	free(str);
	free(copia);
}
