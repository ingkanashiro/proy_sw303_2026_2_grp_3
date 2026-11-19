// structs

/* La estructura Weight contiene la información de una carga sobre la estructura
plana, la cual incluye un puntero de tipo Node* al nodo donde se deposita la
carga y un valor decimal correspondiente a la fuerza en kilonewtons */
typedef struct weight {
  Node* origin;
  double force;
} Weight;

/* La estructura Bar contiene la información de una barra de la estructura
plana, es decir, dos punteros de tipo Node* a los nodos que conecta, junto con
la magnitud de la fuerza interna de la barra en kilonewtons */
typedef struct bar {
  Node *origin, *dest;
  double force;
} Bar;

/* La estructura Node contiene la información de un nodo de la estructura plana,
esto es, sus coordenadas */
typedef struct node {
  double x, y;
} Node;

/* La estructura Support contiene un puntero de tipo Node* el cual apunta al
nodo que  se  apoya,  si  el  apoyo  es  fijo  o  móvil,  y  las  fuerzas  de
reacción  en  ambas dimensiones en kilonewtons */
typedef struct support {
  Node* target;
  unsigned int is_fixed;
  double reaction_x, reaction_y;
} Support;

/* La estructura Data almacena cuatro arreglos de estructuras: nodes, 
bars, weights, supports, junto con el número de elementos en cada una */
typedef struct data {
  Node* nodes;
  Bar* bars;
  Weight* weights;
  Support* supports;
  int n, b, w, s;
} Data;