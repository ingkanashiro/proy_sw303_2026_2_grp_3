#ifndef MODELLING_H
#define MODELLING_H

#include <stdio.h>
#include <stdlib.h> // creo q esto deberia ir en modeling.c
#include "types.h"

/* Recorre  los  datos  recibidos  y  los  interpreta  por  bloques.  Devolverá
una estructura de tipo Data, la cual almacenará la información seccionada por
tipo en cuatro arreglos de estructuras Node, Bar, Weight y Support */
Data interpretData(char** data);

/* Analiza una línea de datos sin procesar y devuelve la estructura 
Nodo correspondiente */
int getNode(char* data, Node* dest);

/* Analiza una línea de datos sin procesar y devuelve la estructura 
Nodo correspondiente */
int getBar(char* data, Bar* dest, Node* nodes, int n);

/* Analiza una línea de datos sin procesar y devuelve la estructura 
Nodo correspondiente */
int getWeight(char* data, Weight* dest, Node* nodes, int n);

/* Analiza una línea de datos sin procesar y devuelve la estructura 
Nodo correspondiente */
int getSupport(char* data, Support* dest, Node* nodes, int n);

#endif