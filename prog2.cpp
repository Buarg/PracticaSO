#include<iostream>
#include<cstring>
#include<fstream>
#include<cstdlib>

using namespace std;

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[]);

int main(int argc, char *argv[])
{
	char c1[256] = "";
	const char DELIMITADOR[] = " \t";
	const int MAX_ELEMENTOS = 21;
	char *trozos[MAX_ELEMENTOS + 1];

	switch(argc)
	{
		case 1: while(strcmp(c1, "salir") != 0) {	
				cout << "Introduce una cadena: " << endl;
				cin.getline(c1, 256);
				cin.clear();

				trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, trozos);

				for(int i = 0; trozos[i] != NULL; i++) {
				cout << trozos[i] << endl;
				}
			}
			break;

		case 2: ifstream e;
			e.open(argv[1]);
			if(e.fail()) {
				cerr << "ERROR: No se puede abrir el archivo" << argv[1] << endl;
				exit(2);
			}
			else {
				e.getline(c1, 255, '\n');
				while(! e.eof()) {
					trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, trozos);
					e.getline(c1, 255, '\n');
				}
				e.close();
			}
			break;

		default: cerr << "ERROR: Demasiados argumentos." << endl;
			 break;
	}

	exit(0);
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
