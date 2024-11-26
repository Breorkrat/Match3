#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include "consts.h"

// Desenha a grade com o cursor na posição x y
void drawGrid(int tabuleiro[LINHAS][COLUNAS], int x, int y, unsigned char selecionado)
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
                DrawRectangleLinesEx(cursor, 3, (selecionado ? RED : YELLOW));
            }
            int ind = tabuleiro[linha][coluna];
            Color cor = CORES[ind];
            DrawCircle(MARGEM+coluna*LADO, MARGEM+linha*LADO, RAIO, cor);
        }
    }
    EndDrawing();
}

// Troca duas peças de posição
void swapCells(int tabuleiro[LINHAS][COLUNAS], int current[2], int final[2]){
    int buffer = tabuleiro[final[1]][final[0]];

    tabuleiro[final[1]][final[0]] = tabuleiro[current[1]][current[0]];
    tabuleiro[current[1]][current[0]] = buffer;
    selecionado = 0;
}

// Atualiza a posição do cursor
void updatePos(int cursor[2], unsigned char selecionado, int tabuleiro[LINHAS][COLUNAS]){
    int old[2] = {cursor[0], cursor[1]};
    if (IsKeyPressed(KEY_RIGHT))
        {
            if (cursor[0] < COLUNAS-1) cursor[0]++;
            if (selecionado) swapCells(tabuleiro, old, cursor);
        }
    else if (IsKeyPressed(KEY_LEFT))
        {
            if (cursor[0] > 0) cursor[0]--;
            if (selecionado) swapCells(tabuleiro, old, cursor);
        }
    if (IsKeyPressed(KEY_UP))
        {
            if (cursor[1] > 0) cursor[1]--;
            if (selecionado) swapCells(tabuleiro, old, cursor);
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (cursor[1] < LINHAS-1) cursor[1]++;
            if (selecionado) swapCells(tabuleiro, old, cursor);
        }
}

void inicializarMatriz(int linhas, int colunas, int tabuleiro[linhas][colunas], int tipos){
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = GetRandomValue(0, NUMTIPOS-1);
        }
    }
}

/*void imprimirMatriz(int matriz[LINHAS][COLUNAS]){
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
}*/

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