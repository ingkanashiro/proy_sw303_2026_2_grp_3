#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

/* Resuelve un sistema de ecuaciones de la forma matricial A x = B, donde A es
una  matriz  cuadrada de orden n, y x y B son matrices de una sola columna de n
elementos

  La salida son los n valores correspondientes a las variables en x. La función
escribe  los  resultados  en  el  arreglo  x.  La  función  devolverá  1 si el
sistema puede resolverse, y 0 en caso contrario */
int solveSystem(double* A, double** x, double* B, int n);

/* Calcula el determinante de una matriz cuadrada A de orden n. La salida es el
resultado del cálculo */
double determinant(double *A, int n);

void printMatrix(double* A, int n, int m);

#endif