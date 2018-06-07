#ifndef abejas
#define abejas

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "leer.h"
#include "benchmark.h"

void crear_empleada (Cfg parametros, int dimf);
void crear_empleada_matriz (Cfg parametros, int dimf);

void exploradora(Cfg parametros, int dimf,int itera_total, int itera);
//void exploradora_matriz(Cfg parametros, int dimf, int itera_total, int itera);
void exploradora_matriz(Cfg parametros, int filas, int colus, int dimf);

void observadora(Cfg parametros, int dimf);
void observadora_matriz(Cfg parametros, int filas, int colus, int dimf);

void actualizar_colonia(Cfg parametros, int dimf);
void actualizar_colonia_matriz(Cfg parametros, int fil, int col, int dimf);

void recombina(Cfg parametros, int i, int k,int par);
void recombina_matriz(Cfg parametros, int fila, int colu, int nfila, int ncolu, int par);

int torneo(Cfg parametros, int dimf);
void torneo_matriz(Cfg parametros, int dimF, int dimC, int& fila, int& colu, int dimf);

void prepara_ruleta(Cfg parametros, double &p_promedio);
void prepara_ruleta_matriz(Cfg parametros, double &p_promedio);

int ruleta(Cfg parametros, int dimf, double p_promedio);
int ruleta_matriz(Cfg parametros, int dimf, double p_promedio);
void conformar_vecindario(int maxF, int maxC, int f, int c, int metodo);
void muestra_vec_vecindario(int dimF, int dimC);
void muestra_vecindario(int dimF, int dimC);
void muestra_vecindario_sol(int dimF, int dimC, int dimf);
void muestra_pos_vecindario(int dimF, int dimC);

#endif
