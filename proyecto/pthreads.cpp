#include <string>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <vector>
#include <math.h>
#include <algorithm>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define N 10000
#define PC 7
#define PM 3
#define E 0.0001
#define T 8

using namespace std;

class Numero {
public:
	bitset<10> enteros;
	bitset<20> decimales;
	double error;

	Numero() {
		enteros = bitset<10> (rand() % 1000);
		decimales = bitset<20> (rand() % 1000000);
	}

	void setError(double original) {
		error = pow(original - pow(toDouble(), 2), 2) / 2;
	}

	double toDouble() {
		string intS = to_string(enteros.to_ulong());
		string decS = to_string(decimales.to_ulong());
		while (decS.length() < 6)
			decS = '0' + decS;
		intS = intS + "." + decS;
		return stod(intS, NULL);
	}
};

vector<Numero> poblacion(N);
double original;

void* generarHijos(void*);
bool comp(Numero n1, Numero n2);

int main () {
	srand((int) time(NULL));
	pthread_t threads[T];

	cout << "Introduce un número para obtener su raíz cuadrada: ";
	cin >> original;

	for (int i = 0; i < N; ++i)
		poblacion[i].setError(original);

	sort(poblacion.begin(), poblacion.end(), comp);

	while (poblacion[0].error > E) {
		for (int i = 0; i < T; ++i)
			pthread_create(&threads[i], NULL, generarHijos, (void*) (intptr_t) i);

		for (int i = 0; i < T; ++i)
			pthread_join(threads[i], NULL);

		sort(poblacion.begin(), poblacion.end(), comp);
		printf("%.6f Error: %f\n", poblacion[0].toDouble(), poblacion[0].error);
	}

	printf("Resultado: %.6f\n", poblacion[0].toDouble());

	return 0;
}

bool comp(Numero n1, Numero n2) {
	return (n1.error < n2.error);
}

void* generarHijos(void* p) {
	int id = (intptr_t) p;
	unsigned int seed = rand();
	Numero *p1 = nullptr, *p2 = nullptr;
	int index = ((T + id) * N) / (2 * T);
	int limiteWrite = ((T + id + 1) * N) / (2 * T);
	int start = (id * N) / (2 * T);
	int limiteRead = ((id + 1) * N) / (2 * T);

	while (index < limiteWrite) {
		for (int i = start; i < limiteRead && index < limiteWrite; ++i) {
			int prob = rand_r(&seed) % 10;
			if (prob < PC) {
				if (!p1)
					p1 = &poblacion[i];
				else {
					p2 = &poblacion[i];
					string s1, s2, sub1, sub2;

					int len = rand_r(&seed) % 10;
					s1 = p1->enteros.to_string<char,string::traits_type,string::allocator_type>();
					s2 = p2->enteros.to_string<char,string::traits_type,string::allocator_type>();
					sub1 = s1.substr(0, len);
					sub2 = s2.substr(0, len);

					s1.replace(0, len, sub2);
					s2.replace(0, len, sub1);
					poblacion[index].enteros = bitset<10> (s1);
					if (index < limiteWrite-1)
						poblacion[index+1].enteros = bitset<10> (s2);

					len = rand_r(&seed) % 20;
					s1 = p1->decimales.to_string<char,string::traits_type,string::allocator_type>();
					s2 = p2->decimales.to_string<char,string::traits_type,string::allocator_type>();
					sub1 = s1.substr(0, len);
					sub2 = s2.substr(0, len);

					s1.replace(0, len, sub2);
					s2.replace(0, len, sub1);
					poblacion[index].decimales = bitset<20> (s1);
					poblacion[index].setError(original);

					if (index < limiteWrite-1) {
						poblacion[index+1].decimales = bitset<20> (s2);
						poblacion[index+1].setError(original);
					}

					for (int j = 0; j < 2 && index+j < limiteWrite; ++j) {
						int probMutar = rand_r(&seed) % 10;
						if (probMutar < PM) {
							if (rand_r(&seed) % 2)
								poblacion[index+j].enteros.flip(rand_r(&seed) % 10);
							else
								poblacion[index+j].decimales.flip(rand_r(&seed) % 20);
							poblacion[index+j].setError(original);
						}
					}

					index += 2;
					p1 = p2 = nullptr;
				}
			}
		}
	}
}

