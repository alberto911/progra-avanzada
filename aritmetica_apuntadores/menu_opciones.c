#include <stdio.h>
#include <stdlib.h>
#define N 3

typedef void (*menu_t)();

void opcion1();
void opcion2();
void opcion3();

int main() {
	int opcion = -1;

	menu_t* opciones = (menu_t*) malloc(N * sizeof(menu_t));
	*opciones = opcion1;
	*(opciones + 1) = opcion2;
	*(opciones + 2) = opcion3;

	while (opcion != 0) {
		printf("1. Opción 1\n2. Opción 2\n3. Opción 3\n0. Salir\n");
		printf("Selecciona: ");
		scanf("%d", &opcion);

		if (opcion > 0 && opcion < 4)
			opciones[opcion - 1]();		
	}

	free(opciones);
	return 0;
}

void opcion1() {
	printf("Seleccionaste la opción 1\n");
}

void opcion2() {
	printf("Seleccionaste la opción 2\n");
}

void opcion3() {
	printf("Seleccionaste la opción 3\n");
}
