#ifndef LOGIC_H
#define LOGIC_H
#include "logic.c"

// Estrutura jogo
typedef struct game game;

// Inicializa tabuleiro e objetivos
void inicializarJogo(game*);

// Desenha o jogo na tela
void draw(game*);

// Checa por matches e os atualiza
void updateMatches(game*);

// Retorna uma screenshot
Texture2D screenshot();

// Desenha um botão
void drawButton(Button);

// Retorna 1 se tiverem matches no tabuleiro
char checarMatches(game);
#endif