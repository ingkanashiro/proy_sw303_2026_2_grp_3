#include "../include/input.h"
#include "../include/modelling.h"
#include <string.h>

#define ERROR_SYNTAX 400
#define ERROR_FORBIDDEN 403
#define ERROR_NOT_FOUND 404

int main(int argc, char** argv) { 

    FILE* file;
    char* path = "input";
    if (argc >= 2) {
        strcpy(path, argv[1]);

        if (argc > 2) {
            printf("[WARNING] Expected 1 argument, but got %d. Extra arguments will be ignored.\n", argc - 1);
        }
    }

    if (!getFile(&file, path)) {
        printf("[ERROR] File '%s' could not be found.\n", path);
        return ERROR_NOT_FOUND;
    }

    char **raw_data = malloc(2048 * 256 * sizeof(char));
    fetchData(file, &raw_data);

    Data data = interpretData(raw_data);

    return 0; 
}