#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"
     
int main(void)     
{  
    SetRandomSeed(time(NULL));
    game tabuleiro;
    tabuleiro.movimentos = 0;  
    tabuleiro.pontos = 0;      
    inicializarMatriz(&tabuleiro);     
    tabuleiro.cursor[0] = GetRandomValue(0, COLUNAS-1);
    tabuleiro.cursor[1] = GetRandomValue(0, LINHAS-1);
    InitWindow(LARGURA, ALTURA+ALTURA_HUD, "Match3");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER)) selecionado = !selecionado;
        updatePos(&tabuleiro, selecionado);
        updateMatches(&tabuleiro);
        draw(tabuleiro, selecionado);
    }
    CloseWindow();
    return 0;
}