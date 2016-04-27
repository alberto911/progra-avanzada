#include <string>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unistd.h>

#define N 1000000
#define PC 7
#define PM 2
#define E 0.0001

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

bool comp(Numero n1, Numero n2) {
	return (n1.error < n2.error);
}

int main () {
	srand((int) time(NULL));
	vector<Numero> poblacion(N);

	double original;
	cout << "Introduce un número para obtener su raíz cuadrada: ";
	cin >> original;

	for (int i = 0; i < N; ++i)
		poblacion[i].setError(original);

	sort(poblacion.begin(), poblacion.end(), comp);

	while (poblacion[0].error > E) {
		Numero *p1 = nullptr, *p2 = nullptr;
		int index = N/2;
		while (index < N) {
			for (int i = 0; i < N/2; ++i) {
				int prob = rand() % 10;
				if (prob < PC) {
					if (!p1) {
						p1 = &poblacion[i];
						//cout << "P1 = " << p1->toDouble() << endl;
					}
					else {
						p2 = &poblacion[i];
						//cout << "P2 = " << p2->toDouble() << endl;
						string s1, s2, sub1, sub2;

						int len = rand() % 10;
						s1 = p1->enteros.to_string<char,string::traits_type,string::allocator_type>();
						s2 = p2->enteros.to_string<char,string::traits_type,string::allocator_type>();
						sub1 = s1.substr(0, len);
						sub2 = s2.substr(0, len);

						s1.replace(0, len, sub2);
						s2.replace(0, len, sub1);
						poblacion[index].enteros = bitset<10> (s1);
						if (index < N-1)
							poblacion[index+1].enteros = bitset<10> (s2);

						len = rand() % 20;		
						s1 = p1->decimales.to_string<char,string::traits_type,string::allocator_type>();
						s2 = p2->decimales.to_string<char,string::traits_type,string::allocator_type>();
						sub1 = s1.substr(0, len);
						sub2 = s2.substr(0, len);						

						s1.replace(0, len, sub2);
						s2.replace(0, len, sub1);
						poblacion[index].decimales = bitset<20> (s1);
						poblacion[index].setError(original);
						//printf("Hijo 1: %.6f %f\n", poblacion[index].toDouble(), poblacion[index].error);
						if (index < N-1) {
							poblacion[index+1].decimales = bitset<20> (s2);
							poblacion[index+1].setError(original);
							//printf("Hijo 2: %.6f %f\n", poblacion[index+1].toDouble(), poblacion[index+1].error);
						}

						for (int j = 0; j < 2 && index+j < N; ++j) {
							int probMutar = rand() % 10;
							//cout << "Prob. mutar(" << j << ") = " << probMutar << endl;
							if (probMutar < PM) {
								poblacion[index+j].enteros.flip();
								poblacion[index+j].decimales.flip();
								poblacion[index+j].setError(original);
								//cout << poblacion[index+j].enteros << " " << poblacion[index+j].decimales << " " << poblacion[index+j].toDouble() << endl;
							}
						}

						index += 2;
						p1 = p2 = nullptr;
						if (index == N)
							break;
					}
				}
			}
		}

		sort(poblacion.begin(), poblacion.end(), comp);
		printf("%.6f Error: %f\n", poblacion[0].toDouble(), poblacion[0].error);
	}

	printf("Resultado: %.6f\n", poblacion[0].toDouble());

	return 0;
}

