#include "logic.c"

typedef struct game game;
void inicializarJogo(game*);
void updatePos(game*, char);
void draw(game*);
void updateMatches(game*);
int checarVazio(game);
Texture2D screenshot();
void drawButton(Button);