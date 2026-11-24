#include <string.h>

#include "../include/equilibrium.h"
#include "../include/input.h"
#include "../include/modelling.h"

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

    printf("%d\n", isBalanced(data.weights, data.w, data.supports, data.s));

    return 0;
}