#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "types.h"

/* Escribe  un  archivo  report.csv, donde  se  detalla  las  fuerzas  internas
en  las barras, las reacciones en los apoyos y el resultado del análisis de la
estructura */
int outputFile(Data data, int isStable);

#endif