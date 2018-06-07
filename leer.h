#ifndef leerArchivo
#define leerArchivo
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
struct Cfg{
	int numero_fuentes;
	int dimension_filas;
	int dimension_columnas;
	int limite_permitido;
	int iteraciones;
	int funcion_o;
	int funcion_dim;
	int seleccion;
	int tamano;
	int recombina;
	float rec_par;
};
Cfg leeArchivo(string nombre);
#endif
