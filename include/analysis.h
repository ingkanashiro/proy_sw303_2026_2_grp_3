#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdlib.h>
#include "types.h"

/* Recibe el número de nodos, barras (equivalente al número de fuerzas de barra)
y el número de reacciones y verifica que existan dos ecuaciones por cada nodo

  La función devolverá 0 si el análisis resulta en un caso isostático, 1 si
resulta en un caso hiperstático y -1 si resulta en un caso hipostático */

#include "types.h"

int analyseIsostatic(int nodes, int bars, int reactions);

/* Recibe  todos  los  datos  del  sistema  y  evalúa  el  sistema  de
ecuaciones. Acomodaremos la matriz de forma que las variables se encuentren
ordenadas: primero todas las fuerzas internas de las barras, seguidas de todas
las reacciones

  La función devolverá 1 en caso de éxito y 0 en caso de fallar */
int evalSystem(Data* data, int nodes, int bars, int reactions);

/* Resuelve un sistema de ecuaciones de la forma matricial A x = B, donde A es
una  matriz  cuadrada de orden n, y x y B son matrices de una sola columna de n
elementos

  La salida son los n valores correspondientes a las variables en x. La función
escribe  los  resultados  en  el  arreglo  x.  La  función  devolverá  1 si el
sistema puede resolverse, y 0 en caso contrario */
int solveSystem(int n, float** A, float** x, float* B);
#endif