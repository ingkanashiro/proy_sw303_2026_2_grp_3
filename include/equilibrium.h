#ifndef EQUILIBRIUM_H
#define EQUILIBRIUM_H

#include "types.h"

/* Recibe  la  información  segmentada  y  determina  si  la  estructura  cumple
la condición de equilibrio, verificando las fuerzas de reacción y los pesos
generados por las cargas. Devuelve 1 si el sistema está en equilibrio, 0 si no
está en equilibrio y -1 si no es posible conocer el estado del sistema (Caso
hiperstático) */
int isBalanced(Weight* weights, int w, Support* supports, int s);

#endif