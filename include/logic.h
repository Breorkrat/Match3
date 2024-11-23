#include <raylib.h>
#include "consts.h"

// Desenha a grade com o cursor na posição x y
void drawGrid(int x, int y)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int linha = 0; linha <= LINHAS; linha++)
    {
        for (int coluna = 0; coluna <= COLUNAS; coluna++)
        {
            if (coluna == x && linha == y) DrawCircle(MARGEM+coluna*LADO, MARGEM+linha*LADO, RAIO, GREEN);
            else DrawCircle(MARGEM+coluna*LADO, MARGEM+linha*LADO, RAIO, RED);
        }
    }
    EndDrawing();
}

// Atualiza a posição do cursor
void updatePos(int cursor[2]){

    if (IsKeyPressed(KEY_RIGHT))
        {
            if (cursor[0] < COLUNAS-1) cursor[0]++;
        }
    else if (IsKeyPressed(KEY_LEFT))
        {
            if (cursor[0] > 0) cursor[0]--;
        }
    if (IsKeyPressed(KEY_UP))
        {
            if (cursor[1] > 0) cursor[1]--;
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (cursor[1] < LINHAS-1) cursor[1]++;
        }
}