#include "logic.c"

typedef struct game game;
void inicializarMatriz(game*);
void updatePos(game*, char);
void swapCells(game*, int[2]);
void drawGrid(game, char);
void checarMatches(game*);
void clearMatchesTable(game*);