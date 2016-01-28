#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* nombre;
	char* apellidos;
	int edad;
	char* telefono;
} paciente;

typedef struct {
	int id;
	paciente* p;
} cama;

int agregarCamas(cama** camas, int** disponibles, int totalCamas, int* totalDisponibles, int n);
void agregarPaciente(cama* camas, int id, int maxLen);
int agregarDisponible(int** disponibles, int id, int total);
int encontrarDisponible(int** disponibles, int* total);

int buscarCama(int total);
void imprimirCama(cama* camas, int id);
void listarPacientes(cama* camas, int total);

void altaPaciente(cama* camas, int id, int** disponibles, int* totalDisponibles);
void liberarCamas(cama* camas, int total);
char* getInput(char* str, int maxLen);

int main() {
	int numInicial, maxLen, totalCamas = 0, totalDisponibles = 0, opcion, id;
	printf("Longitud máxima de strings: ");
	scanf("%d", &maxLen);
	printf("Número inicial de camas: ");
	scanf("%d", &numInicial);

	cama* camas = NULL;
	int* disponibles = NULL;
	totalCamas = agregarCamas(&camas, &disponibles, totalCamas, &totalDisponibles, numInicial);

	do {
		printf("\n1. Incorporar paciente\n2. Buscar cama\n3. Dar de alta a paciente\n4. Listar pacientes\n5. Ver camas disponibles\n0. Salir\nSelecciona: ");
		scanf("%d", &opcion);
		getchar();

		switch(opcion) {
			case 1:
				if (totalDisponibles == 0) {
					printf("Agregando nuevas camas...\n");
					totalCamas = agregarCamas(&camas, &disponibles, totalCamas, &totalDisponibles, 5);
				}
				id = encontrarDisponible(&disponibles, &totalDisponibles);
				agregarPaciente(camas, id, maxLen);
				break;
			case 2:
				id = buscarCama(totalCamas);
				if (id != -1)
					imprimirCama(camas, id);
				break;
			case 3:
				id = buscarCama(totalCamas);
				if (id != -1)
					altaPaciente(camas, id, &disponibles, &totalDisponibles);
				break;
			case 4:
				listarPacientes(camas, totalCamas);
				break;
			case 5:
				printf("Camas disponibles: %d\nCamas ocupadas: %d\n", totalDisponibles, totalCamas - totalDisponibles);
				break;
			case 0:
				break;
			default:
				printf("Opción no válida\n");
		}
	} while (opcion != 0);

	liberarCamas(camas, totalCamas);
	free(disponibles);
	free(camas);
	return 0;
}

int agregarCamas(cama** camas, int** disponibles, int totalCamas, int* totalDisponibles, int n) {
	*camas = realloc(*camas, (totalCamas+n) * sizeof(cama));
	cama *c, *end = *camas + totalCamas + n;

	for (c = *camas + totalCamas; c < end; ++c) {
		c->id = ++totalCamas;
		c->p = NULL;
		*totalDisponibles = agregarDisponible(disponibles, c->id, *totalDisponibles);
	}
	return totalCamas;
}

void agregarPaciente(cama* camas, int id, int maxLen) {
	printf("Cama %d\n", id);
	cama* c = camas + id - 1;
	c->p = (paciente*) malloc(sizeof(paciente));

	c->p->nombre = (char*) malloc(maxLen * sizeof(char));
	c->p->apellidos = (char*) malloc(maxLen * sizeof(char));
	c->p->telefono = (char*) malloc(maxLen * sizeof(char));

	printf("Nombre: ");
	getInput(c->p->nombre, maxLen);
	printf("Apellidos: ");
	getInput(c->p->apellidos, maxLen);
	printf("Edad: ");
	scanf("%d", &c->p->edad);
	getchar();
	printf("Teléfono: ");
	getInput(c->p->telefono, maxLen);
}

int agregarDisponible(int** disponibles, int id, int total) {
	*disponibles = realloc(*disponibles, (total + 1) * sizeof(int));
	*(*disponibles + total) = id;
	return total + 1;
}

int encontrarDisponible(int** disponibles, int* total) {
	int id = *(*disponibles + --(*total));
	*disponibles = realloc(*disponibles, *total * sizeof(int));
	return id;
}

int buscarCama(int total) {
	int id;	
	printf("ID de la cama: ");
	scanf("%d", &id);
	getchar();

	if (id >= 1 && id <= total)
		return id;
	else {
		printf("No hay una cama con ese ID\n");
		return -1;
	}
}

void imprimirCama(cama* camas, int id) {
	cama* c = camas + id - 1;
	if (c->p)
		printf("Paciente: %s %s, %d años, Tel: %s\n", c->p->nombre, c->p->apellidos, c->p->edad, c->p->telefono);
	else
		printf("Cama disponible\n");
}

void listarPacientes(cama* camas, int total) {
	cama* c;
	for (c = camas; c < camas + total; ++c) {
		if (c->p)
			printf("Cama %d -> Paciente: %s %s, %d años, Tel: %s\n", c->id, c->p->nombre, c->p->apellidos, c->p->edad, c->p->telefono);
	}
}

void altaPaciente(cama* camas, int id, int** disponibles, int* totalDisponibles) {
	cama* c = camas + id - 1;
	free(c->p->nombre);
	free(c->p->apellidos);
	free(c->p->telefono);
	free(c->p);
	c->p = NULL;

	*totalDisponibles = agregarDisponible(disponibles, id, *totalDisponibles);
	printf("El paciente fue dado de alta\n");
}

void liberarCamas(cama* camas, int total) {
	cama* c;
	for (c = camas; c < camas + total; ++c) {
		if (c->p) {
			free(c->p->nombre);
			free(c->p->apellidos);
			free(c->p->telefono);
			free(c->p);
		}
	}
}

char* getInput(char* str, int maxLen) {
	fgets(str, maxLen, stdin);
	*(str + strcspn(str, "\n")) = 0;
}
