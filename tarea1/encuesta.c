#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X 10
#define Y 6
#define R 5

typedef struct {
	char* nombre;
	int edad;
	int* respuestas;
} persona;

void obtenerPoblacion(persona* poblacion, int N, int maxLen);
void obtenerCuestionario(char** preguntas, char*** respuestas, int maxLen);

void responderManual(char** preguntas, char*** respuestas, persona* poblacion, int N);
void responderAleatorio(persona* poblacion, int N);

void histograma(persona* poblacion, int N);
void histogramaEdades(persona* poblacion, int N);
int rangoEdad(persona* p);

void liberarPoblacion(persona* poblacion, int N);
void liberarCuestionario(char** preguntas, char*** respuestas);

int main() {
	int N, maxLen, aleatorio;
	printf("Longitud máxima de strings: ");
	scanf("%d", &maxLen);
	printf("Población: ");
	scanf("%d", &N);

	persona* poblacion = (persona*) malloc(N * sizeof(persona));
	obtenerPoblacion(poblacion, N, maxLen);

	char** preguntas = (char**) malloc(X * sizeof(char*));
	char*** respuestas = (char***) malloc(X * sizeof(char**)), ***c;
	for (c = respuestas; c < respuestas + X; ++c)
		*c = (char**) malloc(Y * sizeof(char*));

	getchar();
	obtenerCuestionario(preguntas, respuestas, maxLen);

	printf("Llenar el cuestionario:\n0. Manualmente\n1. Aleatoriamente\nSelecciona: ");
	scanf("%d", &aleatorio);
	if (aleatorio)
		responderAleatorio(poblacion, N);	
	else	
		responderManual(preguntas, respuestas, poblacion, N);
	
	histograma(poblacion, N);
	histogramaEdades(poblacion, N);	

	// Liberar memoria
	liberarPoblacion(poblacion, N);
	free(poblacion);

	liberarCuestionario(preguntas, respuestas);
	for (c = respuestas; c < respuestas + X; ++c)
		free(*c);
	free(respuestas);
	free(preguntas);
	return 0;
}

void obtenerPoblacion(persona* poblacion, int N, int maxLen) {
	persona* p;
	for (p = poblacion; p < poblacion + N; ++p) {
		p->nombre = (char*) malloc(maxLen * sizeof(char));
		p->respuestas = (int*) malloc(X * sizeof(int));
		
		do {
			printf("Persona %ld\n", p - poblacion + 1);
			printf("Nombre: ");
			getchar();
			fgets(p->nombre, maxLen, stdin);
			printf("Edad: ");
			scanf("%d", &p->edad);

			if (p->edad <= 17 || p->edad >= 120) {
				printf("La persona debe tener más de 17 y menos de 120 años.\n");
			}
		} while(p->edad <= 17 || p->edad >= 120);
	}
}

void obtenerCuestionario(char** preguntas, char*** respuestas, int maxLen) {
	char ***pr, **p, **r;
	for (p = preguntas, pr = respuestas; p < preguntas + X; ++p, ++pr) {
		*p = (char*) malloc(maxLen * sizeof(char));
		printf("Pregunta %ld: ", p - preguntas + 1);
		fgets(*p, maxLen, stdin);
		
		for (r = *pr; r < *pr + Y; ++r) {
			*r = (char*) malloc(maxLen * sizeof(char));
			printf("Respuesta %ld: ", r - *pr + 1);
			fgets(*r, maxLen, stdin);
		}
	}
}

void responderManual(char** preguntas, char*** respuestas, persona* poblacion, int N) {
	persona* s;	
	char ***pr, **p, **r;
	int* i;
	for (s = poblacion; s < poblacion + N; ++s) {
		printf("Encuesta de %s", s->nombre);

		for (p = preguntas, pr = respuestas, i = s->respuestas; p < preguntas + X; ++p, ++pr, ++i) {
			printf("Pregunta %ld: %s", p - preguntas + 1, *p);
		
			for (r = *pr; r < *pr + Y; ++r)
				printf("%ld. %s", r - *pr + 1, *r);
			
			printf("> ");
			scanf("%d", i);
		}
	}
}

void responderAleatorio(persona* poblacion, int N) {
	persona* p;
	int* i;
	srand((unsigned) time(NULL));
	
	for (p = poblacion; p < poblacion + N; ++p)
		for (i = p->respuestas; i < p->respuestas + X; ++i)
			*i = (rand() % (Y+1)) + 1; // Y + 1 para simular que algunos no responden una pregunta
}

void histograma(persona* poblacion, int N) {
	persona* p;
	int** resultados = (int**) malloc(X * sizeof(int*)), **r, *i, total, j;
	for (r = resultados; r < resultados + X; ++r) {
		*r = (int*) malloc(Y * sizeof(int));
		for (i = *r; i < *r + Y; ++i)
			*i = 0;
	}

	for (p = poblacion; p < poblacion + N; ++p) {	
		for (r = resultados, i = p->respuestas; r < resultados + X; ++r, ++i) {
			if (*i >= 1 && *i <= Y)
				*(*r + *i - 1) += 1;
		}
	}

	printf("\nHistograma\n");
	for (r = resultados; r < resultados + X; ++r) {
		total = 0;
		printf("\nPregunta %ld\n", r - resultados + 1);
		printf("No.   Respuesta   Frecuencia\n");
		for (i = *r; i < *r + Y; ++i) {
			total += *i;
			printf("%4ld %4d         ", i - *r + 1, *i);
			for (j = 0; j < *i; ++j)
				printf("*");
			printf("\n");
		}
		printf("Total: %d\n", total);
	}

	for (r = resultados; r < resultados + X; ++r)
		free(*r);
	free(resultados);
}

void histogramaEdades(persona* poblacion, int N) {
	persona* p;
	int** resultados = (int**) malloc(X * sizeof(int*)), **r, *i, total, j;
	for (r = resultados; r < resultados + X; ++r) {
		*r = (int*) malloc(R * sizeof(int));
		for (i = *r; i < *r + R; ++i)
			*i = 0;
	}

	int rango;
	for (p = poblacion; p < poblacion + N; ++p) {
		rango = rangoEdad(p);
		for (r = resultados, i = p->respuestas; r < resultados + X; ++r, ++i) {
			if (*i >= 1 && *i <= Y)
				*(*r + rango) += 1;
		}
	}

	printf("\nHistograma de edades\n");
	printf("Rangos:\n1. 18-25\n2. 26-35\n3. 36-45\n4. 46-65\n5. 66-120\n");
	for (r = resultados; r < resultados + X; ++r) {
		printf("\nPregunta %ld\n", r - resultados + 1);
		printf("Rango   Personas   Cantidad\n");
		for (i = *r; i < *r + R; ++i) {
			printf("%4ld  %4d         ", i - *r + 1, *i);
			for (j = 0; j < *i; ++j)
				printf("*");
			printf("\n");
		}
	}

	for (r = resultados; r < resultados + X; ++r)
		free(*r);
	free(resultados);
}

int rangoEdad(persona* p) {
	if (p->edad <= 25)
		return 0;
	else if (p->edad >= 26 && p->edad <= 35)
		return 1;
	else if (p->edad >= 36 && p->edad <= 45)
		return 2;
	else if (p->edad >= 46 && p->edad <= 65)
		return 3;
	else
		return 4;
}

void liberarPoblacion(persona* poblacion, int N) {
	persona* p;
	for (p = poblacion; p < poblacion + N; ++p) {
		free(p->nombre);
		free(p->respuestas);
	}
}

void liberarCuestionario(char** preguntas, char*** respuestas) {
	char ***pr, **p, **r;
	for (p = preguntas, pr = respuestas; p < preguntas + X; ++p, ++pr) {
		free(*p);
		for (r = *pr; r < *pr + Y; ++r)
			free(*r);
	}
}
