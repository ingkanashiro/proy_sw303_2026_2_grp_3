#include "../include/input.h"

int getFile(FILE** dest, char* target) {
    *dest = fopen(target, "r");
    return (*dest == NULL) ? 0 : 1;
}

int fetchData(FILE* file, char*** data) {

    char buffer[256];

    int lines = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        strcpy((*data)[lines], buffer);
        lines++;
    }

    // set end of list as null
    (*data)[lines] = NULL; 

    fclose(file);
    return lines;
}
