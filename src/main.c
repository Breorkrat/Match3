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

void drawGrid();
int updateX(int curX);
int updateY(int curY);

int x, y;

int main(void)
{
    SetRandomSeed(time(NULL));
    x = GetRandomValue(0, COLUNAS-1);
    y = GetRandomValue(0, LINHAS-1);

    InitWindow(LARGURA, ALTURA, "Atividade - Matheus");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        x = updateX(x);
        y = updateY(y);
        drawGrid();
    }
    CloseWindow();
    return 0;
}

void drawGrid()
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

int updateX(int curX){

        if (IsKeyPressed(KEY_RIGHT))
        {
            if (curX < COLUNAS-1) curX ++;
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            if (curX > 0) curX--;
        }
        return curX;
}

int updateY(int curY){

        if (IsKeyPressed(KEY_UP))
        {
            if (curY > 0) curY --;
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (curY < LINHAS-1) curY++;
        }
        return curY;
}
