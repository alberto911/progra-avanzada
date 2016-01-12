#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char nombre[15];
	char apellido[15];
	int edad;
} persona;

int crecerLista(persona** lista, int total, int i) {
	int cantidad;	
	printf("¿Cuántas personas más deseas agregar? ");
	scanf("%d", &cantidad);
	total += cantidad;

	*lista = realloc(*lista, total * sizeof(persona));

	return total;
}

int obtenerDatos(persona* lista, int total, int i) {
	while (i < total) {
		printf("Persona %d\n", i+1);
		printf("Nombre: ");
		scanf("%s", lista[i].nombre);
		printf("Apellido: ");
		scanf("%s", lista[i].apellido);
		printf("Edad: ");
		scanf("%d", &lista[i].edad);
		++i;
	}
	return i;
}

void promedio(persona lista[], int total) {
	float suma = 0;
	int i;
	for (i = 0; i < total; ++i)
		suma += lista[i].edad;

	if (total > 0)
		printf("Promedio de edades: %f\n", suma / total);
}

void masJoven(persona lista[], int total) {
	int menor, i, indice;
	for (i = 0; i < total; ++i) {
		if (i == 0 || lista[i].edad < menor) {
			menor = lista[i].edad;
			indice = i;
		}
	}

	if (total > 0)
		printf("Persona más joven: %s %s\n", lista[indice].nombre, lista[indice].apellido);
}

void masVieja(persona lista[], int total) {
	int mayor, i, indice;
	for (i = 0; i < total; ++i) {
		if (i == 0 || lista[i].edad > mayor) {
			mayor = lista[i].edad;
			indice = i;
		}
	}

	if (total > 0)
		printf("Persona más vieja: %s %s\n", lista[indice].nombre, lista[indice].apellido);
}

void rango(persona lista[], int total) {
	int i, inferior, superior;
	printf("Límite inferior: ");
	scanf("%d", &inferior);
	printf("Límite superior: ");
	scanf("%d", &superior);

	printf("Personas dentro del rango:\n");
	for (i = 0; i < total; ++i) {
		if (lista[i].edad >= inferior && lista[i].edad <= superior)
			printf("  - %s %s\n", lista[i].nombre, lista[i].apellido);
	}
}

int main() {
	int cantidad, i = 0, total = 0;

	printf("¿Cuántas personas deseas agregar? ");
	scanf("%d", &cantidad);
	total += cantidad;
	
	persona* lista = (persona*) malloc(total * sizeof(persona));
	i = obtenerDatos(lista, total, i);

	// Menu de opciones
	int opcion;
	do {
		printf("1. Agregar más personas\n2. Edad promedio\n3. Persona más joven\n4. Persona más vieja\n5. Rango de edades\n");
		printf("0. Salir\nSelecciona una opción: ");
		scanf("%d", &opcion);

		switch (opcion) {
			case 1:
				total = crecerLista(&lista, total, i);
				i = obtenerDatos(lista, total, i);
				break;
			case 2:
				promedio(lista, total);
				break;
			case 3:
				masJoven(lista, total);
				break;
			case 4:
				masVieja(lista, total);
				break;
			case 5:
				rango(lista, total);
				break;
			case 0:
				break;
			default:
				printf("Opción no válida\n");
		}
	} while (opcion != 0);

	free(lista);
	return 0;
}
