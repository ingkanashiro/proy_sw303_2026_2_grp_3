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
            int status;
            int argi1, argi2;
            double argf1, argf2;
            char *args1;

            if (n == 0 && read_mode != R_NODES && read_mode != R_NOTDEF) {
                printf("[ERROR] Nodes section (n) must be declared first.\n");
                // bocchi
            }

            switch (read_mode) {
                case R_NODES:

                    n = getNodes(data + i, &nodes);

                    if (n == -1) {
                        kill_switch = 1; 
                    }

                    i += (n-1);
                    break;

                case R_BARS:
                    
                    b = getBars(data + i, &bars, nodes, n);

                    if (b == -1) {
                        kill_switch = 1; 
                    }

                    i += (b-1);
                    break;

                case R_WEIGHTS:

                    w = getWeights(data + i, &weights, nodes, n);

                    if (w == -1) {
                        kill_switch = 1; 
                    }

                    i += (w-1);
                    break;

                case R_SUPPORTS:

                    s = getSupports(data + i, &supports, nodes, n);

                    if (s == -1) {
                        kill_switch = 1; 
                    }

                    i += (s-1);
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

int getNodes(char** data, Node** nodes) {
    int i = 0;

    while (data[i] != NULL && data[i][0] != ' ' && data[i][0] != '\t') {

        if (i > MAX_NODES) {
            printf("[ERROR] Input for Node is out of bounds. Maximum is %d entries.\n", MAX_NODES);
            return -1;
        } 

        double x, y;
        
        int status = sscanf(data[i], "%lf %lf", &x, &y);
        if (status < 2) {
            printf("[ERROR] Input for Node expected 2 arguments, but got %d.\n", status);
            return -1;
        }
        
        Node node = {
            x, y
        };

        *nodes[i] = node;

        i++;
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