#include <raylib.h>
#include <string.h>
#include <time.h>
#include "include/logic.h"

int main(void)
{
    // Inicializa uma posição aleatória para o cursor
    SetRandomSeed(time(NULL));
    int cursor[2];
    cursor[0] = GetRandomValue(0, COLUNAS-1);
    cursor[1] = GetRandomValue(0, LINHAS-1);

    int tabuleiro[LINHAS][COLUNAS];
    inicializarMatriz(LINHAS, COLUNAS, tabuleiro, NUMTIPOS);
    //int matches[LINHAS][COLUNAS];

    InitWindow(LARGURA, ALTURA, "Match3");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER)) selecionado = !selecionado;
        updatePos(cursor, selecionado, tabuleiro);
        drawGrid(tabuleiro, cursor[0], cursor[1], selecionado);
    }
    CloseWindow();
    return 0;
}