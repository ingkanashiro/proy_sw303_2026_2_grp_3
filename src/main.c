#include <string.h>

#include "../include/analysis.h"
#include "../include/input.h"
#include "../include/modelling.h"
#include "../include/matrix.h"

#define ERROR_SYNTAX 400
#define ERROR_FORBIDDEN 403
#define ERROR_NOT_FOUND 404
#define ERROR_IM_A_TEAPOT 418
#define ERROR_UNPROCESSABLE 422
#define SUCCESS 200 // no lo podemos usar xd, retorniariamos main temprano


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
      printf("[ERROR] cannot know the state of system, amount of weights or supports might be incorrect'\n");
      return ERROR_UNPROCESSABLE;
    }

    if (balanced) {
      printf(
          "the system is balanced, we proceed with the system's evaluation'\n");
    } else {
      printf(
          "[WARNING] system is not balanced, stopping right here'\n");
      return ERROR_UNPROCESSABLE;
    }
    printf("%d\n", data.n);
    printf("%d\n", data.b);
    printf("%d\n", data.s);
    printf("%d\n", data.w);
    printf("%d\n", data.nodes[0].x);
    printf("%d\n", data.nodes[1].x);
    printf("%d\n", data.nodes[2].x);

    evalSystem(&data);

    

    
    return 0;
}