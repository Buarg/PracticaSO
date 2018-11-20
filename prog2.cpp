#include<iostream>
#include<cstring>

using namespace std;

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[]);

int main(int argc, char *argv[])
{
	char c1[256];
	const char DELIMITADOR[] = " \t";
	const int MAX_ELEMENTOS = 21;
	char *trozos[MAX_ELEMENTOS + 1];

	switch(argc)
	{
		case 1: cout << "Introduce una cadena: " << endl;
			cin.getline(c1, 256);
			cin.clear();
			
		case 2: 

		default: cerr << "ERROR: Demasiados argumentos." << endl;
	}
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
