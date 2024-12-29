#include "logic.c"

typedef struct game game;
void inicializarMatriz(game*);
void updatePos(game*, char);
void draw(game*);
void updateMatches(game*);
void updateMousePos(game*);