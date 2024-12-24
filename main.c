#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"
     
int main(void)     
{
    game tabuleiro;         
    inicializarMatriz(&tabuleiro);    
    // Inicializa uma posição aleatória para o cursor
    SetRandomSeed(time(NULL)); 
    tabuleiro.cursor[0] = GetRandomValue(0, COLUNAS-1);
    tabuleiro.cursor[1] = GetRandomValue(0, LINHAS-1);

    InitWindow(LARGURA, ALTURA, "Match3");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER)) selecionado = !selecionado;
        updatePos(&tabuleiro, selecionado);
        checarMatches(&tabuleiro);
        drawGrid(tabuleiro, selecionado);
    }
    CloseWindow();
    return 0;
}