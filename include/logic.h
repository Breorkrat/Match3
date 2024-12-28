#include "logic.c"

typedef struct game game;
void inicializarMatriz(game*);
void updatePos(game*, char);
void draw(game, char);
void updateMatches(game*);