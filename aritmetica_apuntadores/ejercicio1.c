#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char* str = NULL;
	size_t length = 0;
	getline(&str, &length, stdin);

	int i, len = strlen(str);
	char temp;

	for (i = 0; i < len / 2; ++i) {
		temp = *(str + i);
		*(str + i) = *(str + len - i - 1);
		*(str + len - i - 1) = temp; 
	}

	printf("%s\n", str);
	free(str);
}
