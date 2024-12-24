#include "logic.c"

typedef struct game game;
void inicializarMatriz(game*);
void updatePos(game*, char);
void drawGrid(game, char);
void checarMatches(game*);