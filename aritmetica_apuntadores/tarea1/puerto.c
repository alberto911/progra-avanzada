#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* nombre;
	char* apellidos;
	int edad;
	char* rol;
} persona;

typedef struct {
	char* nombre;
	float eslora;
	float manga;
	int maxTripulantes;
	int numTripulantes;
	int propietario;
	int* tripulacion;
} barco;

int agregarBarco(barco** barcos, int total, int maxLen);
void agregarPersona(persona** personas, int total, int maxLen, int propietario);
void asignarTripulante(barco* b, int p);

void imprimirBarcos(barco* barcos, persona* personas, int total);
void imprimirPersonas(barco* b, persona* personas);
void imprimirTripulacion(barco* b, persona* personas);

void liberarBarcos(barco* barcos, int total);
void liberarPersonas(persona* personas, int total);

barco* buscarBarco(barco* barcos, int total, int maxLen);
char* getInput(char* str, int maxLen);

int main() {
	int maxLen, totalBarcos = 0, totalPersonas = 0, opcion, p;
	printf("Longitud máxima de strings: ");
	scanf("%d", &maxLen);

	barco *barcos = NULL, *b;
	persona *personas = NULL;

	do {
		printf("\n1. Agregar barco\n2. Agregar tripulante\n3. Agregar propietario\n4. Mostrar tripulación de un barco\n5. Mostrar todos los barcos\n0. Salir\nSelecciona: ");
		scanf("%d", &opcion);

		switch (opcion) {
			case 1:
				totalBarcos = agregarBarco(&barcos, totalBarcos, maxLen);
				break;
			case 2:
				b = buscarBarco(barcos, totalBarcos, maxLen);
				if (b && b->numTripulantes < b->maxTripulantes) {
					agregarPersona(&personas, totalPersonas, maxLen, 0);
					asignarTripulante(b, totalPersonas++);
				}
				else if (b)
					printf("¡Ya no hay más plazas disponibles!\n");
				break;
			case 3:
				b = buscarBarco(barcos, totalBarcos, maxLen);
				if (b && b->propietario == -1) {
					agregarPersona(&personas, totalPersonas, maxLen, 1);
					b->propietario = totalPersonas++;
				}
				else if (b)
					printf("¡El barco ya tiene un propietario!\n");
				break;
			case 4:
				b = buscarBarco(barcos, totalBarcos, maxLen);
				if (b)
					imprimirPersonas(b, personas);
				break;
			case 5:
				imprimirBarcos(barcos, personas, totalBarcos);
				break;
			case 0:
				break;
			default:
				printf("Opción no válida\n");		
		}
	} while (opcion != 0);

	liberarBarcos(barcos, totalBarcos);
	free(barcos);
	liberarPersonas(personas, totalPersonas);
	free(personas);
	return 0;
}

int agregarBarco(barco** barcos, int total, int maxLen) {
	*barcos = realloc(*barcos, sizeof(barco) * (total + 1));
	barco* b = *barcos + total;

	b->nombre = (char*) malloc(maxLen * sizeof(char));		
	printf("Nombre: ");
	getchar();
	fgets(b->nombre, maxLen, stdin);

	printf("Eslora: ");
	scanf("%f", &b->eslora);
	printf("Manga: ");
	scanf("%f", &b->manga);
	printf("Número máximo de tripulantes: ");
	scanf("%d", &b->maxTripulantes);
	b->numTripulantes = 0;
	b->propietario = -1;
	b->tripulacion = NULL;

	return total + 1;
}

void agregarPersona(persona** personas, int total, int maxLen, int propietario) {
	*personas = realloc(*personas, sizeof(persona) * (total + 1));
	persona* p = *personas + total;

	p->nombre = (char*) malloc(maxLen * sizeof(char));
	p->apellidos = (char*) malloc(maxLen * sizeof(char));
	p->rol = (char*) malloc(maxLen * sizeof(char));
	printf("Nombre: ");
	getInput(p->nombre, maxLen);
	printf("Apellidos: ");
	getInput(p->apellidos, maxLen);

	printf("Edad: ");
	scanf("%d", &p->edad);
	getchar();

	if (!propietario) {
		printf("Rol: ");
		getInput(p->rol, maxLen);
	}
}

void asignarTripulante(barco* b, int p) {
	b->tripulacion = (int*) realloc(b->tripulacion, (b->numTripulantes + 1) * sizeof(int));
	*(b->tripulacion + b->numTripulantes++) = p;
}

barco* buscarBarco(barco* barcos, int total, int maxLen) {
	char* nombre = (char*) malloc(maxLen * sizeof(char));	
	printf("Barco: ");
	getchar();
	fgets(nombre, maxLen, stdin);

	barco* b;
	for (b = barcos; b < barcos + total; ++b) {
		if (strcmp(b->nombre, nombre) == 0) {
			free(nombre);
			return b;
		}
	}

	printf("¡No se encontró el barcó!\n");
	free(nombre);
	return NULL;
}

void imprimirBarcos(barco* barcos, persona* personas, int total) {
	barco* b;
	persona* p;
	int* i;
	for (b = barcos; b < barcos + total; ++b) {
		printf("\nNombre: %s", b->nombre);
		printf("Eslora: %.2f\n", b->eslora);
		printf("Manga: %.2f\n", b->manga);
		printf("Plazas disponibles: %d\n", b->maxTripulantes - b->numTripulantes);

		imprimirTripulacion(b, personas);
	}
}

void imprimirPersonas(barco* b, persona* personas) {
	persona* p = personas + b->propietario;
	if (b->propietario != -1)	
		printf("Propietario: %s %s, %d años\n", p->nombre, p->apellidos, p->edad);
	imprimirTripulacion(b, personas);
}

void imprimirTripulacion(barco* b, persona* personas) {
	persona* p;
	int* i;
	if (b->numTripulantes > 0) {
		printf("Tripulación:\n");
		for (i = b->tripulacion; i < b->tripulacion + b->numTripulantes; ++i) {
			p = personas + *i;
			printf("  %s %s, %d años, %s\n", p->nombre, p->apellidos, p->edad, p->rol);
		}
	}
}

void liberarBarcos(barco* barcos, int total) {
	barco* b;
	for (b = barcos; b < barcos + total; ++b) {
		free(b->nombre);
		free(b->tripulacion);
	}
}

void liberarPersonas(persona* personas, int total) {
	persona* p;
	for (p = personas; p < personas + total; ++p) {
		free(p->nombre);
		free(p->apellidos);
		free(p->rol);
	}
}

char* getInput(char* str, int maxLen) {
	fgets(str, maxLen, stdin);
	*(str + strcspn(str, "\n")) = 0;
}
