#include "raylib.h"
#include <string.h>
#include <time.h>

#define LINHAS 8
#define COLUNAS 10
#define RAIO 20
#define LADO (2*RAIO+10)
#define MARGEM LADO/2
#define LARGURA COLUNAS*LADO
#define ALTURA LINHAS*LADO

void drawGrid(int x, int y);
void updatePos(int cursor[2]);

int main(void)
{
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