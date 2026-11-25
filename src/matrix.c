#include "../include/matrix.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
typedef struct GraphNode {
  int index;
  struct GraphNode** connections;
} GraphNode;

int dfs(int u, GraphNode* rows, int* vis, int* mc) {
  for (int k = 0; rows[u].connections[k] != NULL; k++) {
    int v = rows[u].connections[k]->index;
    if (vis[v]) continue;
    vis[v] = 1;  // FUCK it was this
    if (mc[v] < 0 || dfs(mc[v], rows, vis, mc)) {
      mc[v] = u;
      return 1;
    }
  }
  return 0;
}

void printMatrix(double* A, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%lf\t", A[i * ((n != m) ? m : n) + j]);
    }
    printf("\n");
  }
}
  */

double determinant(double* A, int n) {
  if (n == 3)
    return A[0] * (A[4] * A[8] - A[5] * A[7]) -
           A[1] * (A[3] * A[8] - A[5] * A[6]) +
           A[2] * (A[3] * A[7] - A[4] * A[6]);
  if (n == 2) return A[0] * A[3] - A[1] * A[2];
  if (n == 1) return A[0];
  double s = 0;
  int j = 0;
  for (int i = 0; i < n; i++) {
    double B[n - 1][n - 1];

    for (int ii = 0; ii < n - 1; ii++)
      for (int jj = 0; jj < n - 1; jj++) {
        if (ii < i) {
          if (jj < j)
            B[ii][jj] = A[(ii)*n + (jj)];
          else
            B[ii][jj] = A[(ii)*n + (jj + 1)];
        } else {
          if (jj < j)
            B[ii][jj] = A[(ii + 1) * n + (jj)];
          else
            B[ii][jj] = A[(ii + 1) * n + (jj + 1)];
        }
      }
    // !! esta invertido por algun motivo el signo xd, cambiamos 1 y -1 de lugar
    // !! en el ternario
    s += (((i % 2) ? -1 : 1)) * A[i * n + 0] *
         determinant((double*)B, n - 1);  // carajomierda
  }

  return s;
};

int solveSystem(double* A, int n, double** x, double* B) {
  // validar n > 0
  // asumimos A cuadrado (confia)
  // mediante axioma de transitivdad, `determinant` tambien confia
  /*
    GraphNode rows[n];
    GraphNode cols[n];
    int c[n];

    for (int i = 0; i < n; i++) {
      rows[i].index = i;
      rows[i].connections = malloc((n + 1) * sizeof(GraphNode*));
      cols[i].index = i;
      cols[i].connections = malloc((n + 1) * sizeof(GraphNode*));
      c[i] = 0;
    }

    for (int i = 0; i < n; i++) {
      int k = 0;
      for (int j = 0; j < n; j++) {
        if (fabs(A[i * n + j]) > 1e-6) {
          rows[i].connections[k++] = &cols[j];
          cols[j].connections[c[j]++] = &rows[i];
        }
      }
      rows[i].connections[k] = NULL;
      //
    }

    for (int j = 0; j < n; j++) cols[j].connections[c[j]] = NULL;

    int mc[n];
    for (int i = 0; i < n; i++) mc[i] = -1;
    // mc[i] = 0;
    int matched = 0;
    for (int u = 0; u < n; u++) {
      int vis[n];
      for (int i = 0; i < n; i++) vis[i] = 0;  // XD
      if (dfs(u, rows, vis, mc)) matched++;
    }

    if (matched < n) return 0;

    GraphNode super[n];

    for (int j = 0; j < n; j++) {
      super[j].index = j;
      super[j].connections = malloc((n + 1) * sizeof(GraphNode*));
      int r = mc[j];
      int s = 0;
      for (int k = 0; rows[r].connections[k] != NULL; k++) {
        int v = rows[r].connections[k]->index;
        if (v != j) {
          super[j].connections[s++] = &super[v];
        }
      }
      super[j].connections[s] = NULL;
    }



    // for (int k = 0; k < n; k++) {
    // for (int i = 0; i < n; i++) {
    // printf("%d\n", super[k].connections[i]->index);
    // }
    // WHAT DO I FUCKING DO
    // FUCK
    // }
    for (int i = 0; i < n; i++) {
      free(rows[i].connections);
      free(cols[i].connections);
      free(super[i].connections);
    };
    */

  // Ax = B
  // P = [...]
  //
  //     [ ]
  // Q = [ ]
  //     [ ]
  // PAQ -> triangular superior
  // O(n√m)

  // eliminacion normal, sera optimizada con lo de arriba despues (O(n^3))
  int o = 0;
  for (int k = 0; k < n; k++) {
    int last = n - 1 - o;

    double max = 0.0;
    int maxIdx = 0;
    for (int l = 0; l <= last; l++) {
      if (max < fabs(A[l * n + k])) {
        max = fabs(A[l * n + k]);
        maxIdx = l;
      }
    }

    if (max < 1e-12) return 0;  // CU04
    if (determinant(A, n) < 1e-6) return 0; // CU04

    if (maxIdx != last) {
      for (int m = 0; m < n; m++) {
        double tmp = A[last * n + m];
        A[last * n + m] = A[maxIdx * n + m];
        A[maxIdx * n + m] = tmp;
      }
      double tmpB = B[last];
      B[last] = B[maxIdx];
      B[maxIdx] = tmpB;
    }
    for (int l = 0; l < last; l++) {
      double factor = A[l * n + k] / A[last * n + k];
      for (int m = 0; m < n; m++) {
        A[l * n + m] -= factor * A[last * n + m];
      }
      B[l] -= factor * B[last];
    }
    o++;
  }

  for (int i = 0; i < n; i++) {
    int col = n - 1 - i;
    double sum = 0.0;
    for (int j = col + 1; j < n; j++) {
      sum += A[i * n + j] * (*x)[j];
    }
    (*x)[col] = (B[i] - sum) / A[i * n + col];
  }

  // printf("%d\n", n);
  return 0;
};

