#include "../include/analysis.h"

#include <math.h>
#include <stdio.h>

#define EPSILON 1e-6

double cosBetween(Node A, Node B) {
  double dx = B.x - A.x;
  double dy = B.y - A.y;
  double L = sqrt(dx * dx + dy * dy);
  if (L < 1e-9) {
    return 0.0;
  }
  return dx / L;
}
double sinBetween(Node A, Node B) {
  double dx = B.x - A.x;
  double dy = B.y - A.y;
  double L = sqrt(dx * dx + dy * dy);
  if (L < 1e-9) {
    return 0.0;
  }
  return dy / L;
}

int isBalanced(Weight* weights, int w, Support* supports, int s) {
  if ((!weights && w > 0) || (!supports && s > 0) || w < 0 || s < 0)
    return -1;  // CU08
  double net_force[2] = {0.0, 0.0};
  for (int i = 0; i < w; i++) {
    net_force[1] -=  // ?? posible bug pq @modeling esta implementandolo como si
                     // fuera modulo en si es culpa de @modelo matematico por no
                     // definir bien, pero creo q es culpa de @modulos por no 🧠
        weights[i].force;  // asumimos que esa fuerza SIEMPRE sera negativa

        // * correccion: asumi mal, fuerza siempre sera positiva, asi que restar nomas
    printf("force: -%lf\n", weights[i].force);
  }
  for (int i = 0; i < s; i++) {
    net_force[0] += supports[i].reaction_x;
    net_force[1] += supports[i].reaction_y;
    printf("reaction %dx: %lf\n",i, supports[i].reaction_x);
    printf("reaction %dy: %lf\n",i, supports[i].reaction_y);
  }
  return (fabs(net_force[0]) < EPSILON && fabs(net_force[1]) < EPSILON);
  // no usamos fuerzas axiales ya que se cancelan entre si, no es necesario
  // sumarlas aqui we dont use axial forces since theyll cancel out anyways,
  // so its not necessary really to add them here
}

int analyseIsostatic(int nodes, int bars, int reactions) {
  return ((2 * nodes > bars + reactions) ? -1 : (2 * nodes < bars + reactions));
}

// !! NO TOCAR PORFAVOR
// esta funcion funciona a base de milagros y esperanzas
// no mirar contenido de la funcion, ya que es de naturaleza cuantica
// al ser vista siempre va colapsar en estado de no funcionar
// y el programa nunca compilara por errores misteriosos
// ** si tienes una correccion o bug haz un ticket, no lo intentes arreglar aca
// ?? hazlo en cuanto antes y notificame, porque en una semana se me va olvidar como funciona todo
// ?? no lo hice mantenible pq me gusta dormir almenos 2h al dia
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
  Support** idxSupports = calloc(data->s, sizeof(Support*));
  Weight** idxWeights = calloc(data->w, sizeof(Weight*));

  for (int k = 0; k < data->b; k++) {
    idxBars[k] = &(data->bars[k]);
  }
  for (int k = 0; k < data->s; k++) {
    idxSupports[k] = &(data->supports[k]);
  }
  for (int k = 0; k < data->w; k++) {
    idxWeights[k] = &(data->weights[k]);
  }

  int r = data->b;  // offset pq reacciones va dsps de barras
  for (int i = 0; i < n; i++) {
    printf("wa.%d\n", i);

    // cada nodo genera fila i, i+1
    for (int j = 0; j < data->b; j++) {
      printf("wa.%d.%d\n", i, j);
      if (data->bars[j].origin == (data->nodes + i / 2)) {
        for (int k = 0; k < data->b; k++) {
          printf("wa.%d.%d.%d\n", i, j, k);
          if ((idxBars[k] == &(data->bars[j]))) {
            // printf("wa.if\n", i, j, k);
            A[(i)*n + j] +=
                cosBetween(*(data->bars[j].origin), *(data->bars[j].dest));
            A[(i + 1) * n + j] +=
                sinBetween(*(data->bars[j].origin), *(data->bars[j].dest));
            // A[i * n + k] = k + i/10.0 + 20;
            // A[(i + 1) * n + k] = k + (i+1)/100.0 + 20;
          }
        }
      } else if (data->bars[j].dest == (data->nodes + i / 2)) {
        for (int k = 0; k < data->b; k++) {
          printf("wa.%d.%d.%d\n", i, j, k);
          if ((idxBars[k] == &(data->bars[j]))) {
            // printf("wa.if\n", i, j, k);
            A[(i)*n + j] +=
                cosBetween(*(data->bars[j].dest), *(data->bars[j].origin));
            A[(i + 1) * n + j] +=
                sinBetween(*(data->bars[j].dest), *(data->bars[j].origin));
            // A[i * n + k] = k + i/10.0 + 20;
            // A[(i + 1) * n + k] = k + (i+1)/100.0 + 20;
          }
        }
      }
      // printf("wa.dspsdelif\n");
    }
    // lo mismo para reacciones
    for (int j = 0; j < reacciones; j++) {
      // printf("wa.%d.%d\n", i, j);
      if (data->supports[j].target == (data->nodes + i / 2)) {
        for (int k = 0; k < data->s; k++) {
          // printf("wa.%d.%d.%d\n", i, j, k);
          if ((idxSupports[k] == &(data->supports[j]))) {
            if (data->supports[j].is_fixed)
              A[(i + 1) * n + r++] = 1;  // direccion y
            else {
              A[(i)*n + r++] = 1;        // direccion x
              A[(i + 1) * n + r++] = 1;  // direccion y
            }
            // creo que deberia funcionar
          }
        }
      }
    }
    for (int j = 0; j < data->w; j++) {
      // printf("wa.%d.%d\n", i, j);
      if (data->weights[j].origin == (data->nodes + i / 2)) {
        for (int k = 0; k < data->s; k++) {
          // printf("wa.%d.%d.%d\n", i, j, k);
          if ((idxWeights[k] == &(data->weights[j]))) {
            B[(i + 1)] = data->weights[j].force;  // direccion y
          }
        }
      }
    }
    i++;
  }

  printMatrix(A, n, n);
  printMatrix(B, n, 1);
  printf("%d\n\n", data->n);

  solveSystem(A, &x, B, n);
  // printf("wa");
  for (int i = 0; i < n; i++) {
    printf("%.2lf\n", x[i]);
  }  // por fin
  free(A);
  free(B);
  // obtenidas las fuerzas (en kilonewtons), estas son las respuestas que
  // buscamos, guardandolo en un csv y todo dsps si una fuerza es negativa es
  // compresion, si es positiva es traccion todas las reacciones verticales
  // siempre seran no negativas ya que estan apuntando hacia arriba reemplazar
  // las fuerzas en cada dato en data
  for (int i = 0; i < data->b; i++) {
    idxBars[i]->force = x[i];
  }
  r = 0;
  for (int i = 0; i < data->s; i++) {
    printf("wa");
    if (idxSupports[i]->is_fixed) {
      idxSupports[i]->reaction_y = x[(r++) + data->b];
      idxSupports[i]->reaction_x = x[(r++) + data->b];
    }
    else {
      idxSupports[i]->reaction_x = 0.0;
      idxSupports[i]->reaction_y = x[(r++) + data->b];
    }

  }
  free(x);
  return 1;
}

