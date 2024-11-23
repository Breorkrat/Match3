#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include "consts.h"

// Desenha a grade com o cursor na posição x y
void drawGrid(int tabuleiro[LINHAS][COLUNAS], int x, int y)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int linha = 0; linha < LINHAS; linha++)
    {
        for (int coluna = 0; coluna < COLUNAS; coluna++)
        {
            if (coluna == x && linha == y) 
            {
                Rectangle cursor = {coluna*LADO, linha*LADO, LADO, LADO};
                DrawRectangleLinesEx(cursor, 3, YELLOW);
            }
            int ind = tabuleiro[linha][coluna];
            Color cor = CORES[ind];
            fflush(stdout);
            DrawCircle(MARGEM+coluna*LADO, MARGEM+linha*LADO, RAIO, cor);
        }
        printf("\n");
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

void inicializarMatriz(int linhas, int colunas, int tabuleiro[linhas][colunas], int tipos){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = GetRandomValue(0, NUMTIPOS-1);
        }
    }
}

void imprimirMatriz(int matriz[LINHAS][COLUNAS]){
    char texto[LINHAS];
    int j;
    BeginDrawing();
    for (int i = 0; i < LINHAS-1; i++) {
        for (j = 0; j < COLUNAS-1; j++) {
            texto[j] = '0'+matriz[i][j];
        }
        DrawText(texto, 0, i*LADO, 20, WHITE);
    }
    EndDrawing();
}

/*int checarMatches(int linhas, int colunas, int tabuleiro[linhas][colunas], int matches[linhas][colunas]){
    int quantMatches = 0;
    // Limpa matriz matches
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matches[i][j] = 0;
        }
    }

    // Checagem na horizontal
    for (int i = 1; i < linhas-1; i++) {
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j] == tabuleiro[i][j-1] && tabuleiro[i][j] == tabuleiro[i][j+1]) {
                matches[i][j] = 1;
                quantMatches++;
            }
        }
    }

    // Checagem na vertical
    for (int i = 1; i < linhas; i++) {
        for (int j = 0; j < colunas-1; j++) {
            if (tabuleiro[i][j] == tabuleiro[i+1][j] && tabuleiro[i][j] == tabuleiro[i-1][j]) {
                    matches[i][j] = 1;
                    quantMatches ++;
            }
        }
    }

    return quantMatches;
}*/