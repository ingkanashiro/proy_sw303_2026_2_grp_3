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


//   ____    _____   ____    ____    _____    ____      _      _____   _____   ____ 
// |  _ \  | ____| |  _ \  |  _ \  | ____|  / ___|    / \    |_   _| | ____| |  _ \ 
// | | | | |  _|   | |_) | | |_) | |  _|   | |       / _ \     | |   |  _|   | | | |
// | |_| | | |___  |  __/  |  _ <  | |___  | |___   / ___ \    | |   | |___  | |_| |
// |____/  |_____| |_|     |_| \_\ |_____|  \____| /_/   \_\   |_|   |_____| |____/ 

/* Recorre los datos recibidos, correspondientes a los nodos de la estructura, y
los interpreta  en un arreglo de estructuras Node, el cual guarda en el
parámetro nodes. Devuelve la cantidad de nodos interpretados */
int getNodes(char** data, Node** nodes);

/* Recorre los datos recibidos, correspondientes a las barras de la estructura,
y los interpreta  en  un  arreglo  de  estructuras  Bar,  el  cual  guarda  en
el  parámetro  bars. Devuelve la cantidad de barras interpretadas */
int getBars(char** data, Bar** bars, Node* nodes, int n);

/* Recorre los datos recibidos, correspondientes a las cargas de la estructura, y
los interpreta en un arreglo de estructuras Weight, el cual guarda en el
parámetro weights. Devuelve la cantidad de cargas interpretadas. */
int getWeights(char** data, Weight** weights, Node* nodes, int n);

/* Recorre los datos recibidos, correspondientes a los apoyos de la estructura,
y los  interpreta en un arreglo de estructuras Support, el cual guarda en el
parámetro supports. Devuelve la cantidad de apoyos interpretados */
int getSupports(char** data, Support** supports, Node* nodes, int n);

#endif