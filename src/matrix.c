#include "../include/matrix.h"

int solveSystem(float* A, float** x, float* B, int n) {
  // validar n > 0
  // asumimos A cuadrado (confia)
  // mediante axioma de transitivdad, `determinant` tambien confia
  /*
  
  9 0 2
  3 6 4
  3 5 7
  
  */
  
  
  for (int k = 0; k < n; k++) {
    A[k * n];
    // WHAT DO I FUCKING DO
    // FUCK
  }
  return 0;
};

float determinant(float* A, int n) {
  if (n == 1) return A[0];
  float s = 0;
  int j = 0;
  for (int i = 0; i < n; i++) {
    float B[n - 1][n - 1];

    for (int ii = 0; ii < n - 1; ii++)
      for (int jj = 0; jj < n - 1; jj++) {
        if (ii < i) {
          if (jj < j) {
            B[ii][jj] = A[(ii)*n + (jj)];
          } else {
            B[ii][jj] = A[(ii)*n + (jj + 1)];
          }
        } else {
          if (jj < j) {
            B[ii][jj] = A[(ii + 1) * n + (jj)];
          } else {
            B[ii][jj] = A[(ii + 1) * n + (jj + 1)];
          }
        }
      }
    // !! esta invertido por algun motivo el signo xd, cambiamos 1 y -1 de lugar
    // en el ternario
    s += (((i % 2) ? -1 : 1)) * A[i * n + 0] *
         determinant((float*)B, n - 1);  // carajomierda
  }

  return s;
};