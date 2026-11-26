#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "matrix.h"
#include "types.h"

/* Recibe  la  información  segmentada  y  determina  si  la  estructura  cumple
la condición de equilibrio, verificando las fuerzas de reacción y los pesos
generados por las cargas. Devuelve 1 si el sistema está en equilibrio, 0 si no
está en equilibrio y -1 si no es posible conocer el estado del sistema (Caso
hiperstático) */
int isBalanced(Weight* weights, int w, Support* supports, int s);

/* Recibe el número de nodos, barras (equivalente al número de fuerzas de barra)
y el número de reacciones y verifica que existan dos ecuaciones por cada nodo

  La función devolverá 0 si el análisis resulta en un caso isostático, 1 si
resulta en un caso hiperstático y -1 si resulta en un caso hipostático */
int analyseIsostatic(int nodes, int bars, int reactions);

/* Recibe  todos  los  datos  del  sistema  y  evalúa  el  sistema  de
ecuaciones. Acomodaremos la matriz de forma que las variables se encuentren
ordenadas: primero todas las fuerzas internas de las barras, seguidas de todas
las reacciones

  La función devolverá 1 en caso de éxito y 0 en caso de fallar */
int evalSystem(Data* data);

#endif