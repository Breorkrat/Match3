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

    InitWindow(LARGURA, ALTURA, "Match3");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        updatePos(cursor);
        drawGrid(cursor[0], cursor[1]);
    }
    CloseWindow();
    return 0;
}