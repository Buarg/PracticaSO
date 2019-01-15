#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

void trocear_linea(char linea[], const char delim[], const int maxElems, char *lista_elementos[]);
void ejecutar(char* lista_arg[]);
void redir(char* lista_arg[]);
void tub(char* lista_arg[]);

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
				redir(lista_arg);

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

					redir(lista_arg);

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
		tub(lista_arg);
	}
}

void redir(char* lista_arg[])
{
	int fd;
	int stdin_copy = dup(0);
	int stdout_copy = dup(1);
	int i = 0;

	while(lista_arg[i + 1] != NULL) {
		if(strcmp(lista_arg[i], "<") == 0) {
			close(0);
			fd = open(lista_arg[i + 1], O_RDONLY);
			if (fd != 0) {
				cerr << "ERROR: No puedo abrir el archivo con el descriptor 0\n";
				exit(-1);
			}
			lista_arg[i] = NULL;
		}
		else {
			if(strcmp(lista_arg[i], ">") == 0) {
				close(1);
				fd = creat(lista_arg[i + 1], 0644);
				if(fd != 1) {
					cerr << "ERROR: No puedo crear el archivo con el descriptor 1\n";
					exit(-1);
				}
				lista_arg[i] = NULL;
			}
		}
		i++;
	}
	ejecutar(lista_arg);
	close(fd);
	dup2(stdin_copy, 0);
	dup2(stdout_copy, 1);
	close(stdin_copy);
	close(stdout_copy);
}

void tub(char* lista_arg[])
{
	int tuberia[2];
	bool contiene = false;
	int i = 0;
	int j = 0;
	int k = 0;
	char* lista_arg2[MAX_ELEMENTOS + 1];

	while(lista_arg[i + 1] != NULL) {
		if(strcmp(lista_arg[i], "|") == 0) {
			contiene = true;
			lista_arg[i] = NULL;
			i++;
			j = i;
			while(lista_arg[j] != NULL) {
				lista_arg2[k] = lista_arg[j];
				j++;
				k++;
			}
			lista_arg2[k] = NULL;
			if(pipe(tuberia) == -1) {
				perror("ERROR: no he podido crear la tuberia");
				exit(-1);
			}
		}
		i++;
	}
	int valor = fork();
	if(contiene == true) {
        switch(valor) {
            case -1:
                perror("ERROR: error al ejecutar fork");
                exit(-1);
            case 0:
                close(tuberia[0]);
                if(dup2(tuberia[1], 1) == -1) {
                    perror("ERROR: no se puede redireccionar stdout");
                    exit(-1);
                }
                if(execvp(lista_arg[0], lista_arg) == -1) {
                    perror("ERROR: error al ejecutar el primer mandato");
                    exit(-1);
                }
            default:
                close(tuberia[1]);
                if(dup2(tuberia[0], 0) == -1) {
                    perror("ERROR: no se puede redireccionar stdin");
                    exit(-1);
                }
                wait(NULL);
                if(execvp(lista_arg2[0], lista_arg2) == -1) {
                    perror("ERROR: error al ejecutar el segundo mandato");
                    exit(-1);
                }
        }
	}
	if(contiene == false && valor == 0) {
		execvp(lista_arg[0], lista_arg);
	}
    wait(NULL);
}
