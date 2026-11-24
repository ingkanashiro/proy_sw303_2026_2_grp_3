#include "../include/equilibrium.h"

#include <math.h>

#define EPSILON 1e-6

int isBalanced(Weight* weights, int w, Support* supports, int s) {
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
  // so its not necessary really to add them here no se como llegariamos
  // aqui, pero se daria CU08
  return -1;  // CU08
};