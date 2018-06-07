#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <iostream>
#include <math.h>
#include "TypeDef.h"
#define abss(a)     (a<0 ? (-a) : a)
using namespace std;

double evaluar_fitness(Sol &x,  int func_number, int dim);
void muestraSol(Sol &x, int dim);
double Shifted_Sphere( int dim , double* x );
double Schwefel_Problem( int dim , double* x );
double Shifted_Rosenbrock( int dim , double* x );
double Shifted_Rastrigin( int dim , double* x );
double Shifted_Griewank( int dim , double* x );
double Shifted_Ackley( int dim , double* x );
double Shifted_Weierstrass( int dim , double* x );
//funciones para F7 CEC 2008
double Fast_Fractal (int dim, double* x);
double twist(double y);
double fractal1D(double x);
double doubledip(double x, double c, double s);

void inicializacion (Sol &lista, int funcion, int dim);
void reinicia (Sol &lista, int funcion, int dim, int itera_total, int itera);

int max_min (int funcion);

void controlar_limite (Sol &lista, int funcion, int dim);

double tiempo(time_t inicial);

int Random_Entero(int min, int max);

double Random_Real(double min, double max);

#endif
