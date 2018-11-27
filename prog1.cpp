#include<iostream>
#include<cstring>

using namespace std;

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[]);

int main()
{
	char c1[256];
	const char DELIMITADOR[] = " \t";
	const int MAX_ELEMENTOS = 21;
	char *trozos[MAX_ELEMENTOS + 1];

	cout << "Introduce una cadena de caracteres" << endl;
	cin.getline(c1, 256);
	cin.ignore();
	cin.clear();

	trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, trozos);

	for(int i = 0; trozos[i] != NULL; i++) {
		cout << trozos[i] << endl;
	}

	return 0;
}

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[])
{
	char *trozo;
	int num = 0;

	while(linea != NULL && num < maxElems - 1) {
		trozo = strsep(&linea, delim);
		if(strcmp(trozo, "") != 0) {
			lista_elementos[num++] = trozo;
		}
	}
	lista_elementos[num] = NULL;
}
