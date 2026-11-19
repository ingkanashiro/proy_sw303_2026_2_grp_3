#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>

/* Busca el archivo de entrada, con el nombre especificado en char *target, y
guarda el puntero de tipo FILE* en dest. Devolverá 1 en caso de éxito y 0 en
caso de fallar */
int getFile(FILE* dest, char* target);

/* Recibe el puntero de archivo FILE *file y guarda cada línea de dicho archivo
en un arreglo dinámico char **data. La información no será procesada */
int fetchData(FILE* file, char** data);

#endif