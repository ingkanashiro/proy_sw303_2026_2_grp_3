#include <string.h>

#include "../include/analysis.h"
#include "../include/input.h"
#include "../include/modelling.h"
#include "../include/matrix.h"

#define ERROR_SYNTAX 400
#define ERROR_FORBIDDEN 403
#define ERROR_NOT_FOUND 404
#define ERROR_IM_A_TEAPOT 418

int main(int argc, char **argv) {
    FILE *file;
    char path[64] = "input.txt";
    if (argc >= 2) {
        strcpy(path, argv[1]); // !! wtf se raya con char pointer

        if (argc > 2) {
        printf(
            "[WARNING] Expected 1 argument, but got %d. Extra arguments will be "
            "ignored.\n",
            argc - 1);
        }
    }

    if (!getFile(&file, path)) {
        printf("[ERROR] File '%s' could not be found.\n", path);
        return ERROR_NOT_FOUND;
    }

    char **raw_data = (char **)malloc(2048 * 256 * sizeof(char));
    fetchData(file, &raw_data);
    Data data = interpretData(raw_data);

    int balanced = isBalanced(data.weights, data.w, data.supports, data.s);
    if (balanced == -1) {
      printf("[ERROR] Cannot know the state of system, amount of weights or supports might be incorrect'\n");
      return ERROR_FORBIDDEN; // FORBIDDEN de manera provisional
    }

    if (balanced) {
      // yay
    } else {
      // nay
    }

    int n = 3;
    double* A = calloc(n * n, sizeof(double));
    A[0] = 2.0f;
    A[1] = 3.0f;
    A[2] = -1.0f;
    A[3] = 4.0f;
    A[4] = -1.0f;
    A[5] = 2.0f;
    A[6] = -1.0f;
    A[7] = 5.0f;
    A[8] = 3.0f;
    printf("%lf\n", determinant(A, n));
    double* x = malloc(n * sizeof(double));
    double* B = calloc(n, sizeof(double));
    B[0] = -9.0;
    B[1] = 2.0;
    B[2] = 17.0;
    solveSystem(A, n, &x, B);

    // imprime los valores de las variables
    for (int i = 0; i < n; i++) {
      printf("x[%d] = %lf\n", i, x[i]);
    }

    free(A);
    free(x);
    free(B);
    return 0;
}