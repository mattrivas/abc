#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <new>
#include <string.h>
#include <limits>
#include <time.h>
#include <float.h>
#include "TypeDef.h"
#include "leer.h"
#include "benchmark.h"
#include "abejas.h"
#include "ABC.h"

using namespace std;

void algoritmo_ABC (Cfg parametros);
void almacenar(int iteraccion, Fuente mejor_solucion, int dimf);

int main (int argc, char *argv[]){
	Cfg param;
	param=leeArchivo(argv[1]);
	param.funcion_o=atoi(argv[2]);
	param.funcion_dim=atoi(argv[3]);
	algoritmo_ABC(param);
	return 1;
}

extern double numEval;
/* EL VECINDARIO TIENE POSICIONES NO SOLUCIONES */
/* VER CUANDO hace la mejor solucion para determinar la cantidad de iteraciones */
extern int **vec_vecindario;
extern Fuente **matrizColonia;
extern Fuente **matrizColoniaAux;
extern Fuente mejor_fuente;

time_t t_inicial;
double t_mejor, t_total, bestEval;

void algoritmo_ABC(Cfg parametros){
	
	srand(time(NULL)*10000);
	srand48(time(NULL)*10000);

	int iteraccion = 0;
	int a = 0,generacion = 1;
	int dimf = parametros.funcion_dim;
	int dimFilas = parametros.dimension_filas;
	int dimColus = parametros.dimension_columnas;
	parametros.limite_permitido = dimf;

	mejor_fuente.solucion = (double*) calloc (dimf, sizeof(double));
	matrizColonia = (Fuente **)malloc(dimFilas * sizeof(Fuente *));
	matrizColoniaAux = (Fuente **)malloc(dimFilas * sizeof(Fuente *));
	/* EL VECINDARIO TIENE POSICIONES NO FUENTES */
	int tot = 18;
	vec_vecindario = (int **)malloc(tot * sizeof(int));
	for(int k=0; k<tot; k++){
		vec_vecindario[k] = (int *)malloc(1 * sizeof(int));
	}
	/*
	vec_vecindario[1] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[2] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[3] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[4] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[5] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[6] = (int *)malloc(1 * sizeof(int));
	vec_vecindario[7] = (int *)malloc(1 * sizeof(int));*/

	mejor_fuente.fitness = (max_min(parametros.funcion_o) == 0)? DBL_MAX : -DBL_MAX;
	
	for(int f=0; f<tot/2; f++){
		for(int c=0; c<2; c++){
			vec_vecindario[f][c] = 0;
		}
	}
	for(int f=0; f<dimFilas; f++){
		matrizColonia[f] = (Fuente *)malloc(dimFilas * sizeof(Fuente));
		matrizColoniaAux[f] = (Fuente *)malloc(dimFilas * sizeof(Fuente));
	}
	for(int f=0; f<dimFilas; f++){
		for(int c=0; c<dimColus; c++){
			matrizColonia[f][c].solucion = (double*) calloc (dimColus, sizeof(double));
			matrizColonia[f][c].limite = 0;
			matrizColoniaAux[f][c].solucion = (double*) calloc (dimColus, sizeof(double));
			matrizColoniaAux[f][c].limite = 0;
		}
	}
	for(int f=0; f<dimFilas; f++){
		for(int c=0; c<dimColus; c++){
			inicializacion(matrizColonia[f][c].solucion, parametros.funcion_o, dimf);
		}
	}
	for(int f=0; f<dimFilas; f++){
		for(int c=0; c<dimColus; c++){
			matrizColonia[f][c].fitness = evaluar_fitness(matrizColonia[f][c].solucion, parametros.funcion_o,dimf);
		}
	}
	//cout << " Fitness inicial: " << endl;
	//muestra_vecindario(dimFilas, dimColus);
	crear_empleada_matriz(parametros, dimf);
	observadora_matriz(parametros, dimFilas, dimColus, dimf);
	time(&t_inicial);
	numEval=bestEval=0.0;
	double totalEval=dimf*5000;
	while(numEval < totalEval){
		crear_empleada_matriz(parametros, dimf);
		observadora_matriz(parametros, dimFilas, dimColus, dimf);
		exploradora_matriz(parametros, dimFilas, dimColus, dimf);
		for(int i=0; i<dimFilas; i++){
			for(int j=0; j<dimColus; j++){
				a = max_min(parametros.funcion_o);
				if (a==0){
					if (matrizColonia[i][j].fitness < mejor_fuente.fitness){
						mejor_fuente.fitness = matrizColonia[i][j].fitness;
						iteraccion = generacion;
						t_mejor = tiempo(t_inicial);
						bestEval = numEval;
						for (int l=0; l<dimf; l++){
							mejor_fuente.solucion[l] = matrizColonia[i][j].solucion[l];
						}
					}
				}else{
					if (matrizColonia[i][j].fitness > mejor_fuente.fitness){
						mejor_fuente.fitness = matrizColonia[i][j].fitness;
						iteraccion = generacion;
						t_mejor = tiempo(t_inicial);
						bestEval = numEval;
						for (int k=0; k<dimf; k++){
							mejor_fuente.solucion[k] = matrizColonia[i][j].solucion[k];
						}
					}
				}
			}
			/*cout << "Generacion: " << generacion << endl;
			if(!(generacion%50)){
				cout << " Fitness final: " << endl;
				muestra_vecindario(dimFilas, dimColus);
			}*/
			generacion = generacion + 1;
		}
		t_total = tiempo(t_inicial);
		double promedio = 0.0;
		for(int i=0; i<dimFilas; i++){
			for(int j=0; j<dimColus; j++){
				promedio += matrizColonia[i][j].fitness;
			}
		}
		promedio = promedio / (dimFilas*dimColus);
		cout << "El promedio es: " << promedio << endl;
	}
	//cout << " Fitness final: " << endl;
	//muestra_vecindario(dimFilas, dimColus);
	cout << "Mejor: " << mejor_fuente.fitness << endl;
	cout << "Iteracion:  " << iteraccion << endl;
}
