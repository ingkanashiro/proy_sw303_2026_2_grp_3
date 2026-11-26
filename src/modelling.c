#include "../include/modelling.h"

#define R_NODES 0
#define R_BARS 1
#define R_WEIGHTS 2
#define R_SUPPORTS 3
#define R_NOTDEF -1

#define MAX_NODES 256
#define MAX_BARS 512
#define MAX_WEIGHTS 128
#define MAX_SUPPORTS 128

Data interpretData(char** data) {

    int n = 0, b = 0, w = 0, s = 0;

    Node *nodes = calloc(MAX_NODES, sizeof(Node));
    Bar *bars = calloc(MAX_BARS, sizeof(Bar));
    Weight *weights = calloc(MAX_WEIGHTS, sizeof(Weight));
    Support *supports = calloc(MAX_SUPPORTS, sizeof(Support));

    int read_mode = R_NOTDEF; 

    int kill_switch = 0;
    for (int i = 0; data[i] != NULL; i++) {

        if (data[i][0] == '?') continue;

        if (kill_switch) {
            nodes = NULL; bars = NULL; weights = NULL; supports = NULL;
            n = -1; b = -1; w = -1; s = -1;
            break;
        }
        
        if (data[i][0] == ' ' || data[i][0] == '\t') {

            if (n == 0 && read_mode != R_NODES && read_mode != R_NOTDEF) {
                printf("[ERROR] Nodes section (n) must be declared first.\n");
                // bocchi
            }

            switch (read_mode) {
                case R_NODES:
                    if (!getNode(data[i], nodes + n)) {
                        printf("(explodes)\n\n");
                        kill_switch = 1;
                    }

                    n++;

                    break;

                case R_BARS:
                    if (!getBar(data[i], bars + b, nodes, n)) {
                        printf("(explodes)\n\n");
                        kill_switch = 1;
                    }

                    b++;
                    break;

                case R_WEIGHTS:
                    if (!getWeight(data[i], weights + w, nodes, n)) {
                        printf("(explodes)\n\n");
                        kill_switch = 1;
                    }

                    w++;
                    break;

                case R_SUPPORTS:
                    if (!getSupport(data[i], supports + s, nodes, n)) {
                        printf("(explodes)\n\n");
                        kill_switch = 1;
                    }

                    s++;
                    break;

                case R_NOTDEF:
                    printf("[ERROR] Element entry orphaned. Perhaps a section is missing?\n");
                    break;
            }
        }
        else {
            // we expect first char to be identifier and second char to be ':' token.
            char id = data[i][0];
            
            if (data[i][1] != ':') {
                printf("[ERROR] Expected token ':' after section identifier. Perhaps identifier is too long?\n");
                // wuwuwu
            }

            switch (id) {
                case 'n':   read_mode = R_NODES;        break;
                case 'b':   read_mode = R_BARS;         break;
                case 'w':   read_mode = R_WEIGHTS;      break;
                case 's':   read_mode = R_SUPPORTS;     break;
                
                default:    
                    read_mode = R_NOTDEF;
                    printf("[ERROR] Identifier '%c' doesn't match any section.\n", id);
                    // err
            }
        }
    }

    Data out = {
        nodes, bars, weights, supports,
        n, b, w, s
    };

    return out;
}

int getNode(char *data, Node *dest) {
    double x, y;
        
    int status = sscanf(data, "%lf %lf", &x, &y);
    if (status < 2) {
        printf("[ERROR] Input for Node expected 2 arguments, but got %d.\n", status);
        return 0;
    }
    
    Node node = {
        x, y
    };

    *dest = node;
    return 1;
}

int getBar(char *data, Bar *dest, Node *nodes, int n) {
    int origin, destination;
        
    int status = sscanf(data, "%d %d", &origin, &destination);
    if (status < 2) {
        printf("[ERROR] Input for Bar expected 2 arguments, but got %d.\n", status);
        return 0;
    }

    if (origin >= n || destination >= n) {
        printf("[ERROR] No such node exists.\n");
        return 0;
    }
    
    Bar bar = {
        .origin = nodes + origin,
        .dest = nodes + destination
    };

    *dest = bar;
    return 1;
}

int getWeight(char *data, Weight *dest, Node *nodes, int n) {
    int origin;
    double force;
        
    int status = sscanf(data, "%d %lf", &origin, &force);
    if (status < 2) {
        printf("[ERROR] Input for Weight expected 2 arguments, but got %d.\n", status);
        return 0;
    }

    if (origin >= n) {
        printf("[ERROR] No such node exists.\n");
        return 0;
    }
    
    Weight weight = {
        .origin = nodes + origin,
        .force = force,
    };

    *dest = weight;
    return 1;
}

int getSupport(char *data, Support *dest, Node *nodes, int n) {
    int target;
    char postsymbol[2] = "";

    int status = sscanf(data, "%d %1[^\n]", &target, postsymbol);
    if (status < 1) {
        printf("[ERROR] Input for Support expected 1 arguments, but got %d.\n", status);
        return 0;
    }

    if (target >= n) {
        printf("[ERROR] No such node exists.\n");
        return 0;
    }
    
    Support support = {
        .target = nodes + target,
        .is_fixed = (postsymbol[0] == '*') ? 1 : 0,
    };

    *dest = support;
    return 1;
}

//   ____    _____   ____    ____    _____    ____      _      _____   _____   ____ 
// |  _ \  | ____| |  _ \  |  _ \  | ____|  / ___|    / \    |_   _| | ____| |  _  \ 
// | | | | |  _|   | |_) | | |_) | |  _|   | |       / _ \     | |   |  _|   | | | |
// | |_| | | |___  |  __/  |  _ <  | |___  | |___   / ___ \    | |   | |___  | |_| |
// |____/  |_____| |_|     |_| \_\ |_____|  \____| /_/   \_\   |_|   |_____| |____/ 

// we might ignore these functions, or make singular analysis for just one element instead.

/* Recorre los datos recibidos, correspondientes a los nodos de la estructura, y
los interpreta  en un arreglo de estructuras Node, el cual guarda en el
parámetro nodes. Devuelve la cantidad de nodos interpretados */
int getNodes(char** data, Node** nodes) {
    
    int i = 0;

    // i wanna try something
    // we can't read like a string array because it's a char** and C doesn't have a dimension for (char*)

    char buffer[256];

    printf("TYADGUHGKPOĹOKIYUFHIJOKPSJOIHABGUYFVUAHYJOKLÑ\n");
    printf("%x %x %x %x", **data, *(*data + 1), *(*data + 2), *(*data + 3));

    sscanf(*data + 1, "%255[^\n\0]", buffer);
    printf("%s", buffer);


    while ((sscanf(*data, "%255[^\n\0]", buffer)) == 1 && (buffer[0] == ' ' || buffer[0] == '\t')) {

        // if (i > MAX_NODES) {
        //     printf("[ERROR] Input for Node is out of bounds. Maximum is %d entries.\n", MAX_NODES);
        //     return -1;
        // } 

        printf("%s", buffer);
        double x, y;
        
        int status = sscanf(buffer, "%lf %lf", &x, &y);
        if (status < 2) {
            printf("[ERROR] Input for Node expected 2 arguments, but got %d.\n", status);
            return -1;
        }
        
        Node node = {
            x, y
        };

        *nodes[i++] = node;
    }

    return ++i;
}

int getBars(char** data, Bar** bars, Node* nodes, int n) {
    int i = 0;

    while (data[i] != NULL && data[i][0] != ' ' && data[i][0] != '\t') {
        
        if (i > MAX_BARS) {
            printf("[ERROR] Input for Bar is out of bounds. Maximum is %d entries.\n", MAX_BARS);
            return -1;
        } 
        
        int origin, dest;
        
        int status = sscanf(data[i], "%d %d", &origin, &dest);
        if (status < 2) {
            printf("[ERROR] Input for Bar expected 2 arguments, but got %d.\n", status);
            return -1;
        }

        if (origin >= n || dest >= n) {
            printf("[ERROR] Invalid argument for Bar. No such node exists.\n");
            return -1;
        }

        Bar bar = {
            .origin = nodes + origin,
            .dest = nodes + dest 
        };

        *bars[i] = bar;
        i++;
    }

    return ++i;
}

int getWeights(char** data, Weight** weights, Node* nodes, int n) {
    int i = 0;

    while (data[i] != NULL && data[i][0] != ' ' && data[i][0] != '\t') {
        
        if (i > MAX_WEIGHTS) {
            printf("[ERROR] Input for Weight is out of bounds. Maximum is %d entries.\n", MAX_WEIGHTS);
            return -1;
        } 

        int origin;
        double force;

        int status = sscanf(data[i], "%d %lf", &origin, &force);
        if (status < 2) {
            printf("[ERROR] Input for Weight expected 2 arguments, but got %d.\n", status);
            return -1;
        }

        if (origin > n) {
            printf("[ERROR] Invalid argument for Weight. No such node exists.\n");
            return -1;
        }

        Weight weight = {
            nodes + origin,
            force
        };

        *weights[i] = weight;
        i++;
    }

    return ++i;
}

int getSupports(char** data, Support** supports, Node* nodes, int n) {
    int i = 0;

    while (data[i] != NULL && data[i][0] != ' ' && data[i][0] != '\t') {

        if (i > MAX_SUPPORTS) {
            printf("[ERROR] Input for Support is out of bounds. Maximum is %d entries.\n", MAX_SUPPORTS);
            return -1;
        } 

        int target;
        char *mark = NULL;

        int status = sscanf(data[i], "%d %s", &target, mark);
        if (status < 1) {
            printf("[ERROR] Input for Support expected 1 argument, but got %d.\n", status);
            return -1;
        }

        if (target > n) {
            printf("[ERROR] Invalid argument for Support. No such node exists.\n");
            return -1;
        }

        Support support = {
            .target = nodes + target,
            .is_fixed = (mark[0] == '*') ? 0 : 1
        };

        *supports[i] = support;
        i++;
    }

    return ++i;
}