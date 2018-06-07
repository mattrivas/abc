#include <stdio.h>
#include <stdlib.h>
#include "abejas.h"
#include "benchmark.h"

Fuente **matrizColonia, **matrizColoniaAux;
int **vec_vecindario;
double fitnes_previo;
int limite_fit;
Fuente mejor_fuente;

void crear_empleada_matriz(Cfg parametros, int dimf){
	int k;	
	int dimFilas = parametros.dimension_filas; 
	int dimColus = parametros.dimension_columnas;
	int enteroF = 0, enteroC = 0;
	for (int f = 0; f < dimFilas; f++){
		for(int c = 0; c < dimColus; c++){
			int nfila = Random_Entero(0,dimFilas);
			int ncolu = Random_Entero(0,dimColus);
			recombina_matriz(parametros, f, c, nfila, ncolu, 1);
		}
	}
	actualizar_colonia_matriz(parametros, dimFilas, dimColus, dimf);
}

void recombina_matriz(Cfg parametros, int f, int c, int filaux, int colaux, int par){
	int dimf = parametros.funcion_dim;
	switch(par){
		/* Recombinación tradicional en ABC */
		case 1: 
		{       
			int posRan = Random_Entero(0,dimf);
			for (int j=0; j<dimf; j++){	
				if(posRan == j){
					//cout << "Dentro de recombina" << " Aux: " << matrizColoniaAux[f][c].solucion[j] << " Matriz colonia: " <<  matrizColonia[f][c].solucion[j] << endl;
					//cout << "Desplazamiento: " << matrizColonia[f][c].solucion[j] + (-1 + drand48()*(1+1)) * (matrizColonia[f][c].solucion[j] - matrizColonia[filaux][colaux].solucion[j]) << endl;
					matrizColoniaAux[f][c].solucion[j] = matrizColonia[f][c].solucion[j] + (-1 + drand48()*(1+1)) * (matrizColonia[f][c].solucion[j] - matrizColonia[filaux][colaux].solucion[j]);
					//cout << " Aux luego de asignacion: " << matrizColoniaAux[f][c].solucion[j] << endl;
				}
				else
					matrizColoniaAux[f][c].solucion[j] = matrizColonia[f][c].solucion[j];		
			}
			controlar_limite (matrizColoniaAux[f][c].solucion, parametros.funcion_o,parametros.funcion_dim);
			matrizColoniaAux[f][c].fitness = evaluar_fitness(matrizColoniaAux[f][c].solucion, parametros.funcion_o,parametros.funcion_dim);			
			/* * * Esto es un PARCHE * * */
			/*
			if(fitnes_previo == matrizColoniaAux[f][c].fitness){
				limite_fit = limite_fit+1;
				if(limite_fit == 3){
					//cout << "Limite FIT" << endl;
					limite_fit = 0;
					inicializacion(matrizColoniaAux[f][c].solucion, parametros.funcion_o, dimf);
					matrizColoniaAux[f][c].fitness = evaluar_fitness(matrizColoniaAux[f][c].solucion, parametros.funcion_o,parametros.funcion_dim);
					//cout << "MatrizColoniaAux Fitness: " << matrizColoniaAux[f][c].fitness << endl;
				}
			}else{
				fitnes_previo = matrizColoniaAux[f][c].fitness;
			}*/
			/*if(max_min(parametros.funcion_o)== 0){
				if(matrizColoniaAux[f][c].fitness<=matrizColonia[f][c].fitness){
					matrizColonia[f][c].fitness = matrizColoniaAux[f][c].fitness;
					matrizColonia[f][c].limite = 0;
					for (int j=0; j<dimf; j++)
						matrizColonia[f][c].solucion[j] = matrizColoniaAux[f][c].solucion[j];
				}else matrizColonia[f][c].limite ++; 
			}else{
				if(matrizColoniaAux[f][c].fitness>matrizColonia[f][c].fitness){
					matrizColonia[f][c].fitness = matrizColoniaAux[f][c].fitness;
					matrizColonia[f][c].limite = 0;
					for (int j=0; j<dimf; j++)
						matrizColonia[f][c].solucion[j] = matrizColoniaAux[f][c].solucion[j];
				}else matrizColonia[f][c].limite ++; 				
			}*/
		}
		break;
		default: printf("no recombina");
	}
}
void observadora_matriz(Cfg parametros, int dimF, int dimC, int dimf){
	int k, i;
	for(int i=0; i<dimF;i++){
		for(int j=0; j<dimC; j++){
			conformar_vecindario(dimF, dimC, i, j, 2);
			int nfila = i;
			int ncolu = j;
			torneo_matriz(parametros, dimF, dimC, nfila, ncolu, dimf);
			recombina_matriz(parametros, i, j, nfila, ncolu, 1);
		}
	}
	actualizar_colonia_matriz(parametros,dimF, dimC, dimf);
}

void actualizar_colonia_matriz(Cfg parametros,int fila, int colu, int dimf){
	for(int i=0; i<fila; i++){
		for(int j=0; j<colu; j++){
			if(max_min(parametros.funcion_o)== 0){
				//cout << "Comparo: " << matrizColonia[i][j].fitness << " con: " << matrizColoniaAux[i][j].fitness << endl;
				if (matrizColoniaAux[i][j].fitness <= matrizColonia[i][j].fitness){
					matrizColonia[i][j].fitness = matrizColoniaAux[i][j].fitness;
					matrizColonia[i][j].limite = 0;
					for (int x=0; x<dimf; x++){
						matrizColonia[i][j].solucion[x] = matrizColoniaAux[i][j].solucion[x];
					}
				}
				else{
					matrizColonia[i][j].limite = matrizColonia[i][j].limite + 1;
				}
			}
			else{
				if (matrizColoniaAux[i][j].fitness > matrizColonia[i][j].fitness) {
					matrizColonia[i][j].fitness = matrizColoniaAux[i][j].fitness;
					matrizColonia[i][j].limite = 0;
					for (int x=0; x<dimf; x++){
						matrizColonia[i][j].solucion[x] =matrizColoniaAux[i][j].solucion[x];
					}
				}else{
					matrizColonia[i][j].limite = matrizColonia[i][j].limite + 1;
				}
			}
		}
	}
}

void torneo_matriz(Cfg parametros, int maxF, int maxC, int& f, int& c, int dimf){
	int t, q;
	t = Random_Entero(0,8);
	q = Random_Entero(0,8);
	if(max_min(parametros.funcion_o)== 0){
		//	cout << "Voy a comparar: " << matrizColonia[vec_vecindario[q][0]][vec_vecindario[q][1]].fitness << " , " << matrizColonia[vec_vecindario[t][0]][vec_vecindario[t][1]].fitness << endl;
		if (matrizColonia[vec_vecindario[q][0]][vec_vecindario[q][1]].fitness > matrizColonia[vec_vecindario[t][0]][vec_vecindario[t][1]].fitness){
		//	cout << "Ganador: " << matrizColonia[vec_vecindario[t][0]][vec_vecindario[t][1]].fitness << endl;
			f = vec_vecindario[t][0];
			c = vec_vecindario[t][1];
		}else{
		//	cout << "Ganador: " << matrizColonia[vec_vecindario[q][0]][vec_vecindario[q][1]].fitness << endl;
			f = vec_vecindario[q][0];
			c = vec_vecindario[q][1];
		}
	}else{
		if (matrizColonia[f][c].fitness < matrizColonia[vec_vecindario[t][0]][vec_vecindario[t][1]].fitness){
			f = vec_vecindario[t][0];
			c = vec_vecindario[t][1];
		}
	}
}

//void exploradora_matriz(Cfg parametros, int dimf, int itera_total, int itera){
void exploradora_matriz(Cfg parametros, int filas, int colus, int dimf){
	//double randomize;
	for(int i=0; i<filas; i++){
		for(int j=0; j<colus; j++){
			if (matrizColonia[i][j].limite >= parametros.limite_permitido){
				if(matrizColonia[i][j].fitness != mejor_fuente.fitness){
					inicializacion(matrizColonia[i][j].solucion, parametros.funcion_o,parametros.funcion_dim);
					matrizColonia[i][j].fitness = evaluar_fitness (matrizColonia[i][j].solucion, parametros.funcion_o,parametros.funcion_dim );
				//cout << " Fitness exploradora  " << matrizColonia[i][j].fitness << endl;
				}
				matrizColonia[i][j].limite = 0;
			}
		}
	}
}
void conformar_vecindario(int maxF, int maxC, int f, int c, int metodo){
	//cout << "Muestro vecindario comun: " << endl;
	//muestra_vecindario(maxF, maxC);
	//cout << "Fila: " << f << " Colu: " << c << endl;
	switch(metodo){
		case 1:
		default:
		if(f == 0){
			if(c == 0){
				vec_vecindario[0][0] = f+1;
				vec_vecindario[0][1] = c;
				vec_vecindario[1][0] = f;
				vec_vecindario[1][1] = maxC-1;
				vec_vecindario[2][0] = maxF-1;
				vec_vecindario[2][1] = c;
				vec_vecindario[3][0] = f;
				vec_vecindario[3][1] = c+1;
				//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
				//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
				//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
				//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
			}else{
				if(c == maxC - 1){
					vec_vecindario[0][0] = f+1;
					vec_vecindario[0][1] = c;
					vec_vecindario[1][0] = f;
					vec_vecindario[1][1] = c-1;
					vec_vecindario[2][0] = maxF-1;
					vec_vecindario[2][1] = c;
					vec_vecindario[3][0] = 0;
					vec_vecindario[3][1] = 0;
					//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
					//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
					//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
					//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;

				}else{
					vec_vecindario[0][0] = f+1;
					vec_vecindario[0][1] = c;
					vec_vecindario[1][0] = f;
					vec_vecindario[1][1] = c-1;
					vec_vecindario[2][0] = maxF-1;
					vec_vecindario[2][1] = c;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = c+1;
					//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
					//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
					//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
					//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
				}
			}
		}else{
			if(f == maxF - 1){
				if(c == 0){
					vec_vecindario[0][0] = 0;
					vec_vecindario[0][1] = c;
					vec_vecindario[1][0] = f;
					vec_vecindario[1][1] = maxC-1;
					vec_vecindario[2][0] = f-1;
					vec_vecindario[2][1] = c;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = 1;
					//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
					//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
					//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
					//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
				}else{
					if(c == maxC - 1){
						vec_vecindario[0][0] = 0;
						vec_vecindario[0][1] = c;
						vec_vecindario[1][0] = f;
						vec_vecindario[1][1] = c-1;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = 0;
						//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
						//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
						//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
						//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
					}else{
						vec_vecindario[0][0] = 0;
						vec_vecindario[0][1] = c;
						vec_vecindario[1][0] = f;
						vec_vecindario[1][1] = c-1;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = c+1;
						//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
						//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
						//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
						//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
					}
				}
			}else{
				if(c == 0){
					vec_vecindario[0][0] = f+1;
					vec_vecindario[0][1] = c;
					vec_vecindario[1][0] = f;
					vec_vecindario[1][1] = maxC-1;
					vec_vecindario[2][0] = f-1;
					vec_vecindario[2][1] = c;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = c+1;
					//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
					//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
					//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
					//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;

				}else{
					if(c == maxC - 1){
						vec_vecindario[0][0] = f+1;
						vec_vecindario[0][1] = c;
						vec_vecindario[1][0] = f;
						vec_vecindario[1][1] = c-1;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = 0;
						//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
						//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
						//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
						//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
					}else{
						vec_vecindario[0][0] = f+1;
						vec_vecindario[0][1] = c;
						vec_vecindario[1][0] = f;
						vec_vecindario[1][1] = c-1;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = c+1;
						//cout << "Abajo: " << matrizColonia[vec_vecindario[0][0]][vec_vecindario[0][1]].fitness << endl;
						//cout << "Izquierda: " << matrizColonia[vec_vecindario[1][0]][vec_vecindario[1][1]].fitness << endl;
						//cout << "Arriba: " << matrizColonia[vec_vecindario[2][0]][vec_vecindario[2][1]].fitness << endl;
						//cout << "Derecha: " << matrizColonia[vec_vecindario[3][0]][vec_vecindario[3][1]].fitness << endl;
					}
				}
			}
		}
		break;
		case 2:
		if(f == 0){
			if(c == 0){
				vec_vecindario[0][0] = maxF-1;
				vec_vecindario[0][1] = maxC-1;
				vec_vecindario[1][0] = maxF-1;
				vec_vecindario[1][1] = c;
				vec_vecindario[2][0] = maxF-1;
				vec_vecindario[2][1] = c+1;
				vec_vecindario[3][0] = f;
				vec_vecindario[3][1] = c+1;
				vec_vecindario[4][0] = f+1;
				vec_vecindario[4][1] = c+1;
				vec_vecindario[5][0] = f+1;
				vec_vecindario[5][1] = c;
				vec_vecindario[6][0] = f+1;
				vec_vecindario[6][1] = maxC-1;
				vec_vecindario[7][0] = f;
				vec_vecindario[7][1] = maxC-1;
				vec_vecindario[8][0] = f;
				vec_vecindario[8][1] = c;

			}else{
				if(c == maxC - 1){
					vec_vecindario[0][0] = maxF-1;
					vec_vecindario[0][1] = c-1;
					vec_vecindario[1][0] = maxF-1;
					vec_vecindario[1][1] = c;
					vec_vecindario[2][0] = maxF-1;
					vec_vecindario[2][1] = 0;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = 1;
					vec_vecindario[4][0] = f+1;
					vec_vecindario[4][1] = 0;
					vec_vecindario[5][0] = f+1;
					vec_vecindario[5][1] = c;
					vec_vecindario[6][0] = f+1;
					vec_vecindario[6][1] = c-1;
					vec_vecindario[7][0] = f;
					vec_vecindario[7][1] = c-1;
					vec_vecindario[8][0] = f;
					vec_vecindario[8][1] = c;
				}else{
					vec_vecindario[0][0] = maxF-1;
					vec_vecindario[0][1] = c-1;
					vec_vecindario[1][0] = maxF-1;
					vec_vecindario[1][1] = c;
					vec_vecindario[2][0] = maxF-1;
					vec_vecindario[2][1] = c+1;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = c+1;
					vec_vecindario[4][0] = f+1;
					vec_vecindario[4][1] = c+1;
					vec_vecindario[5][0] = f+1;
					vec_vecindario[5][1] = c;
					vec_vecindario[6][0] = f+1;
					vec_vecindario[6][1] = c-1;
					vec_vecindario[7][0] = f;
					vec_vecindario[7][1] = c-1;
					vec_vecindario[8][0] = f;
					vec_vecindario[8][1] = c;
				}
			}
		}else{
			if(f == maxF - 1){
				if(c == 0){

					vec_vecindario[0][0] = f-1;
					vec_vecindario[0][1] = maxC-1;
					vec_vecindario[1][0] = f-1;
					vec_vecindario[1][1] = c;
					vec_vecindario[2][0] = f-1;
					vec_vecindario[2][1] = c+1;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = c+1;
					vec_vecindario[4][0] = 0;
					vec_vecindario[4][1] = c+1;
					vec_vecindario[5][0] = 0;
					vec_vecindario[5][1] = c;
					vec_vecindario[6][0] = 0;
					vec_vecindario[6][1] = maxC-1;
					vec_vecindario[7][0] = f;
					vec_vecindario[7][1] = maxC-1;
					vec_vecindario[8][0] = f;
					vec_vecindario[8][1] = c;
				}else{
					if(c == maxC - 1){
						vec_vecindario[0][0] = f-1;
						vec_vecindario[0][1] = c-1;
						vec_vecindario[1][0] = f-1;
						vec_vecindario[1][1] = c;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = 0;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = 0;
						vec_vecindario[4][0] = 0;
						vec_vecindario[4][1] = 0;
						vec_vecindario[5][0] = 0;
						vec_vecindario[5][1] = c;
						vec_vecindario[6][0] = 0;
						vec_vecindario[6][1] = c-1;
						vec_vecindario[7][0] = f;
						vec_vecindario[7][1] = c-1;
						vec_vecindario[8][0] = f;
						vec_vecindario[8][1] = c;
					}else{
						vec_vecindario[0][0] = f-1;
						vec_vecindario[0][1] = c-1;
						vec_vecindario[1][0] = f-1;
						vec_vecindario[1][1] = c;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c+1;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = c+1;
						vec_vecindario[4][0] = 0;
						vec_vecindario[4][1] = c+1;
						vec_vecindario[5][0] = 0;
						vec_vecindario[5][1] = c;
						vec_vecindario[6][0] = 0;
						vec_vecindario[6][1] = c-1;
						vec_vecindario[7][0] = f;
						vec_vecindario[7][1] = c-1;
						vec_vecindario[8][0] = f;
						vec_vecindario[8][1] = c;
					}
				}
			}else{
				if(c == 0){
					vec_vecindario[0][0] = f-1;
					vec_vecindario[0][1] = maxC-1;
					vec_vecindario[1][0] = f-1;
					vec_vecindario[1][1] = c;
					vec_vecindario[2][0] = f-1;
					vec_vecindario[2][1] = c+1;
					vec_vecindario[3][0] = f;
					vec_vecindario[3][1] = c+1;
					vec_vecindario[4][0] = f+1;
					vec_vecindario[4][1] = c+1;
					vec_vecindario[5][0] = f+1;
					vec_vecindario[5][1] = c;
					vec_vecindario[6][0] = f+1;
					vec_vecindario[6][1] = maxC-1;
					vec_vecindario[7][0] = f;
					vec_vecindario[7][1] = maxC-1;
					vec_vecindario[8][0] = f;
					vec_vecindario[8][1] = c;
				}else{
					if(c == maxC - 1){
						vec_vecindario[0][0] = f-1;
						vec_vecindario[0][1] = c-1;
						vec_vecindario[1][0] = f-1;
						vec_vecindario[1][1] = c;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = 0;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = 0;
						vec_vecindario[4][0] = f+1;
						vec_vecindario[4][1] = 0;
						vec_vecindario[5][0] = f+1;
						vec_vecindario[5][1] = c;
						vec_vecindario[6][0] = f+1;
						vec_vecindario[6][1] = c-1;
						vec_vecindario[7][0] = f;
						vec_vecindario[7][1] = c-1;
						vec_vecindario[8][0] = f;
						vec_vecindario[8][1] = c;
					}else{
						vec_vecindario[0][0] = f-1;
						vec_vecindario[0][1] = c-1;
						vec_vecindario[1][0] = f-1;
						vec_vecindario[1][1] = c;
						vec_vecindario[2][0] = f-1;
						vec_vecindario[2][1] = c+1;
						vec_vecindario[3][0] = f;
						vec_vecindario[3][1] = c+1;
						vec_vecindario[4][0] = f+1;
						vec_vecindario[4][1] = c+1;
						vec_vecindario[5][0] = f+1;
						vec_vecindario[5][1] = c;
						vec_vecindario[6][0] = f+1;
						vec_vecindario[6][1] = c-1;
						vec_vecindario[7][0] = f;
						vec_vecindario[7][1] = c-1;
						vec_vecindario[8][0] = f;
						vec_vecindario[8][1] = c;
					}
				}
			}
		}
		break;
	}
}

void muestra_vecindario(int dimF, int dimC){
	for(int i=0; i<dimF; i++){
		for(int j=0; j<dimC; j++){
			cout << " " << matrizColonia[i][j].fitness;
		}
		cout << endl;
	}
}
void muestra_vecindario_sol(int dimF, int dimC, int dimf){
	for(int i=0; i<dimF; i++){
		for(int j=0; j<dimC; j++){
			for(int k=0; k<dimf; k++)
				cout << " " << matrizColonia[i][j].solucion[k] <<endl;
		}
		cout << endl;
	}
}
void muestra_vec_vecindario(int dimF, int dimC){
	for(int i=0; i<dimF; i++){
		for(int j=0; j<dimC; j++){
			cout << " " << vec_vecindario[i][j];
		}
		cout << endl;
	}
}
