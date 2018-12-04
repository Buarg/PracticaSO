#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[]);
void ejecutar(char* lista_arg[]);

const int MAX_CARACTERES = 255;
const int MAX_ELEMENTOS = 20;
const char DELIMITADOR[] = " \t";

int main(int argc, char *argv[])
{
	char c1[MAX_CARACTERES + 1] = "";
	char* lista_arg[MAX_ELEMENTOS + 1];

	switch(argc) {
		case 1:
			cout << "$";
			cin.getline(c1, MAX_CARACTERES);
			cin.clear();

			trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, lista_arg);

			while(strcmp(c1, "salir") != 0) {
				ejecutar(lista_arg);

				cout << "$";
				cin.getline(c1, MAX_CARACTERES);
				cin.clear();

				trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, lista_arg);
			}
			break;

		case 2:
		{
			ifstream e;
			e.open(argv[1]);

			if(e.fail()) {
				cerr << "No puede abrirse el archivo " << argv[1] << endl;
				exit(2);
			}
			else {
				e.getline(c1, MAX_CARACTERES, '\n');
				while(! e.eof()) {
					trocear_linea(c1, DELIMITADOR, MAX_ELEMENTOS, lista_arg);

					ejecutar(lista_arg);

					e.getline(c1, MAX_CARACTERES, '\n');
				}
				e.close();
			}
			break;
		}

		default:
			cerr << "Numero de argumrntos incorrecto" << endl;
			exit(1);			
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

void ejecutar(char* lista_arg[])
{
	if(strcmp(lista_arg[0], "cd") == 0) {
		chdir(lista_arg[1]);
	}
	else { 
		int valor = fork();

		if(valor == 0) {
			if(execvp(lista_arg[0], lista_arg) == -1) {
				perror("Error al ejecutar la funcion execvp \n");
				exit(-1);
			}
		}
	}
	wait(NULL);
}
