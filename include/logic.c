#include <raylib.h>
#include <time.h>
#include <stdio.h>

#define LINHAS 8
#define COLUNAS 10
#define RAIO 20
#define LADO (2*RAIO+10)
#define MARGEM LADO/2
#define LARGURA COLUNAS*LADO
#define ALTURA LINHAS*LADO
#define NUMTIPOS 5

const Color CORES[] = {BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, WHITE};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));
char selecionado = 0;

typedef struct game {
    int tabuleiro[LINHAS][COLUNAS];
    int matches[LINHAS][COLUNAS];
    int cursor[2];
} game;

void inicializarMatriz(game* tabuleiro)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            tabuleiro->tabuleiro[i][j] = GetRandomValue(1, NUMTIPOS);
        }
    }
}

// Troca duas peças de posição
void swapCells(game* tabuleiro, int final[2])
{
    int buffer = tabuleiro->tabuleiro[final[1]][final[0]];

    tabuleiro->tabuleiro[final[1]][final[0]] = tabuleiro->tabuleiro[tabuleiro->cursor[1]][tabuleiro->cursor[0]];
    tabuleiro->tabuleiro[tabuleiro->cursor[1]][tabuleiro->cursor[0]] = buffer;
    selecionado = 0;
}

// Atualiza a posição do cursor
void updatePos(game* tabuleiro, char selecionado)
{
    int old[2] = {tabuleiro->cursor[0], tabuleiro->cursor[1]};
    if (IsKeyPressed(KEY_RIGHT))
    {
        if (tabuleiro->cursor[0] < COLUNAS - 1) tabuleiro->cursor[0]++;
        if (selecionado) swapCells(tabuleiro, old);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        if (tabuleiro->cursor[0] > 0) tabuleiro->cursor[0]--;
        if (selecionado) swapCells(tabuleiro, old);
    }
    if (IsKeyPressed(KEY_UP))
    {
        if (tabuleiro->cursor[1] > 0) tabuleiro->cursor[1]--;
        if (selecionado) swapCells(tabuleiro, old);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        if (tabuleiro->cursor[1] < LINHAS - 1) tabuleiro->cursor[1]++;
        if (selecionado) swapCells(tabuleiro, old);
    }
}

// Desenha a grade com o cursor na posição x y
void drawGrid(game tabuleiro, char selecionado)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int linha = 0; linha < LINHAS; linha++)
    {
        for (int coluna = 0; coluna < COLUNAS; coluna++)
        {
            if(tabuleiro.matches[linha][coluna] == 1) {
                DrawCircleLines(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO + 4, BLUE);
            }
            if (tabuleiro.matches[linha][coluna] == 2) {
                DrawCircleLines(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO + 4, GREEN);
            } 

            // Desenha cursor
            if (coluna == tabuleiro.cursor[0] && linha == tabuleiro.cursor[1])
            {
                Rectangle cursor = {coluna * LADO, linha * LADO, LADO, LADO};
                DrawRectangleLinesEx(cursor, 3, (selecionado ? RED : YELLOW));
            }
            // Desenha peças
            int tab = tabuleiro.tabuleiro[linha][coluna];
            Color cor = CORES[tab];
            DrawCircle(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO, cor);
        }
    }
    EndDrawing();
}

void clearMatches(game* tabuleiro) {
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            tabuleiro->matches[i][j] = 0;
        }
    }
}

void checarMatches(game* tabuleiro)
{
    clearMatches(tabuleiro);

    // Checagem na horizontal
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 1; j < COLUNAS - 1; j++)
        {
            if (tabuleiro->tabuleiro[i][j] == tabuleiro->tabuleiro[i][j - 1] && 
                tabuleiro->tabuleiro[i][j] == tabuleiro->tabuleiro[i][j + 1])
            {
                tabuleiro->matches[i][j] = 1;
            }
        }
    }

    // Checagem na vertical
    for (int i = 1; i < LINHAS - 1; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (tabuleiro->tabuleiro[i][j] == tabuleiro->tabuleiro[i + 1][j] && 
                tabuleiro->tabuleiro[i][j] == tabuleiro->tabuleiro[i - 1][j])
            {
                tabuleiro->matches[i][j] = 2;
            }
        }
    }
}