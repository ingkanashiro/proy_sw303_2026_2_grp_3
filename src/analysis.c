#include "../include/analysis.h"

#include <stdio.h>
#include <math.h>

#include "../include/matrix.h"

#define EPSILON 1e-6

double cosBetween(Node A, Node B);
double sinBetween(Node A, Node B);

int isBalanced(Weight* weights, int w, Support* supports, int s) {
  if ((!weights && w > 0) || (!supports && s > 0) || w < 0 || s < 0)
    return -1;  // CU08
  double net_force[2] = {0.0, 0.0};
  for (int i = 0; i < w; i++)
    net_force[1] +=
        weights[i].force;  // asumimos que esa fuerza SIEMPRE sera negativa
  for (int i = 0; i < s; i++) {
    net_force[0] += supports[i].reaction_x;
    net_force[1] += supports[i].reaction_y;
  }
  return (fabs(net_force[0]) < EPSILON && fabs(net_force[1]) < EPSILON);
  // no usamos fuerzas axiales ya que se cancelan entre si, no es necesario
  // sumarlas aqui we dont use axial forces since theyll cancel out anyways,
  // so its not necessary really to add them here
}

int analyseIsostatic(int nodes, int bars, int reactions) {
  return ((2 * nodes > bars + reactions) ? -1 : (2 * nodes < bars + reactions));
};

int evalSystem(Data* data) {
  // primero analizar isoestaticidad, solo si es isostatico proceder
  // analyseIsostatic requiere la cantidad de nodos, barras y reacciones
  // reacciones se obtiene de sumar 2 * apoyos fijos + apoyos moviles
  int reacciones = 0;
  int s = data->s;
  while (s--) reacciones += 1 + data->supports[data->s - s - 1].is_fixed;

  if (analyseIsostatic(data->n, data->b, reacciones))
    return 0;  // caso de uso creo
  // iterar por los nodos, (hashmap?) contando las reacciones y las fuerzas
  // (siempre sera igual a la cantidad de barras)
  int n = 2 * data->n;
  double* A = calloc(n * n, sizeof(double));
  double* x = malloc(n * sizeof(double));
  double* B = calloc(n, sizeof(double));

  // tenemos que tener un ID (nodo nodo) de las barras, para colocar los cosenos
  // y senos adecuados en cada indice de la barra las cargas van a final de la
  // matriz B con esto formamos la matriz A y B, n para matrix.c sera 2n para
  // analysis.c, le meteremos 2n ya que habran 2n ecuaciones con 2n incognitas
  
  // hacer arreglo de punteros de las barras, y usar eso como indices
  
  Bar** idxBars = calloc(data->b, sizeof(Bar*));
  Support** idxSupports = calloc(data->b, sizeof(Support*));
  
  
  for (int k = 0; k < data->b; k++) {
    idxBars[k] = &(data->bars[k]);
  }
  for (int k = 0; k < data->s; k++) {
    idxSupports[k] = &(data->supports[k]);
  }


  for (int i = 0; i < data->n; i++) {
    printf("wa.%d\n", i);
    
    // cada nodo genera fila i, i+1
    for (int j = 0; j < data->b; j++) {
      // printf("wa.%d.%d\n", i, j);
      if (data->bars[j].dest == (data->nodes+i)) {
        for (int k = 0; k < data->b; k++) {
          // printf("wa.%d.%d.%d\n", i, j, k);
          if (idxBars[k] = &(data->bars[j])) {
            // printf("wa.if\n", i, j, k);
            A[i * n + j] =
            cosBetween(*(data->bars[j].origin), *(data->bars[j].dest));
            A[(i + 1) * n + j] =
            sinBetween(*(data->bars[j].origin), *(data->bars[j].dest));
          }
        }
      }
      // printf("wa.dspsdelif\n");
    }
    // lo mismo para reacciones
    int r = data->b;  // offset pq reacciones va dsps de barras
    for (int j = 0; j < reacciones; j++) {
      // printf("wa.%d.%d\n", i, j);
      if (data->supports[j].target == &(data->nodes[i])) {
        for (int k = 0; k < data->s; k++) {
          // printf("wa.%d.%d.%d\n", i, j, k);
          if (idxSupports[k] = &(data->supports[j])) {
            if (data->supports[j].is_fixed)
              A[(r++) * n + j] = 1;  // direccion x
            A[(r++) * n + j] = 1;    // direccion y
            // creo que deberia funcionar
          }
        }
      }
    }
  }
  
  printMatrix(A, n / 2, n / 2);
  printf("%d", data->n);
  
  solveSystem(A, &x, B, n);
  // printf("wa");
  free(A);
  free(B);
  // obtenidas las fuerzas (en kilonewtons), estas son las respuestas que
  // buscamos, guardandolo en un csv y todo dsps si una fuerza es negativa es
  // compresion, si es positiva es traccion todas las reacciones verticales
  // siempre seran no negativas ya que estan apuntando hacia arriba reemplazar
  // las fuerzas en cada dato en data
  free(x);
  return 1;
}

double cosBetween(Node A, Node B) { return 1.0; };
double sinBetween(Node A, Node B) { return 1.0; };