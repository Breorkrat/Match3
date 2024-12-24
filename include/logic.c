#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define LINHAS 8
#define COLUNAS 10
#define RAIO 20
#define LADO (2*RAIO+10)
#define MARGEM LADO/2
#define LARGURA COLUNAS*LADO
#define ALTURA LINHAS*LADO
#define NUMTIPOS 5

const Color CORES[] = {BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, SKYBLUE, VIOLET};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));
char selecionado = 0;

typedef struct game {
    int tabuleiro[COLUNAS][LINHAS];
    int matches[COLUNAS][LINHAS];
    int cursor[2];
} game;

void inicializarMatriz(game* tabuleiro)
{
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            tabuleiro->tabuleiro[x][y] = GetRandomValue(1, NUMTIPOS);
        }
    }
}

// Troca duas peças de posição
void swapCells(game* tabuleiro, int final[2])
{
    int buffer = tabuleiro->tabuleiro[final[0]][final[1]];

    tabuleiro->tabuleiro[final[0]][final[1]] = tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]];
    tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]] = buffer;
    selecionado = 0;
}

// Atualiza a posição do cursor
void updatePos(game* tabuleiro, char selecionado)
{
    int old[2] = {tabuleiro->cursor[0], tabuleiro->cursor[1]};
    if (IsKeyPressed(KEY_RIGHT))
    {
        // Versão branchless comentada, acho que não é prático para manter no código mas fiz como um exercício
        //tabuleiro->cursor[0] += (tabuleiro->cursor[0] < COLUNAS - 1);
        if (tabuleiro->cursor[0] < COLUNAS - 1) tabuleiro->cursor[0]++;
        if (selecionado) swapCells(tabuleiro, old);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        //tabuleiro->cursor[0] -= (tabuleiro->cursor[0] > 0);
        if (tabuleiro->cursor[0] > 0) tabuleiro->cursor[0]--;
        if (selecionado) swapCells(tabuleiro, old);
    }
    if (IsKeyPressed(KEY_UP))
    {
        //tabuleiro->cursor[1] -= (tabuleiro->cursor[1] > 0);
        if (tabuleiro->cursor[1] > 0) tabuleiro->cursor[1]--;
        if (selecionado) swapCells(tabuleiro, old);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        //tabuleiro->cursor[1] += (tabuleiro->cursor[1] < LINHAS - 1);
        if (tabuleiro->cursor[1] < LINHAS - 1) tabuleiro->cursor[1]++;
        if (selecionado) swapCells(tabuleiro, old);
    }
}

// Desenha a grade com o cursor na posição x y
void drawGrid(game tabuleiro, char selecionado)
{
    BeginDrawing();
    ClearBackground(BLACK);

    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            if(tabuleiro.matches[x][y] == 1) {
                //DrawCircleLines(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO + 4, BLUE);

                //DrawCircleLines(MARGEM + (coluna-1) * LADO, MARGEM + linha * LADO, RAIO + 5, RED);
                //DrawCircleLines(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO + 5, RED);
                //DrawCircleLines(MARGEM + (coluna+1) * LADO, MARGEM + linha * LADO, RAIO + 5, RED);
            }
            if (tabuleiro.matches[x][y] == 2) {
                //DrawCircleLines(MARGEM + coluna * LADO, MARGEM + linha * LADO, RAIO + 4, GREEN);

                //DrawCircleLines(MARGEM + coluna * LADO, MARGEM + (linha+1) * LADO, RAIO + 5, RED);
            } 

            // Desenha cursor
            if (x == tabuleiro.cursor[0] && y == tabuleiro.cursor[1])
            {
                Rectangle cursor = {x * LADO, y * LADO, LADO, LADO};
                DrawRectangleLinesEx(cursor, 3, (selecionado ? RED : YELLOW));
            }
            // Desenha peças
            int tab = tabuleiro.tabuleiro[x][y];
            Color cor = CORES[tab];
            DrawCircle(MARGEM + x * LADO, MARGEM + y * LADO, RAIO, cor);
        }
    }
    EndDrawing();
}

// Limpa a tabela de matches
void clearMatchesTable(game* tabuleiro) {
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            tabuleiro->matches[x][y] = 0;
        }
    }
}

void bubbleClear(game* tabuleiro, int x, int y){
    drawGrid(*tabuleiro, false);
    usleep(200000); // Aguarda 0.2 segundos
    int i;
    for (i = y; i > 0; i--){
        tabuleiro->tabuleiro[x][i] = tabuleiro->tabuleiro[x][i-1];
        tabuleiro->tabuleiro[x][i-1] = 0;
    }
    tabuleiro->tabuleiro[x][i] = GetRandomValue(1, NUMTIPOS);
}

void checarMatches(game* tabuleiro)
{
    clearMatchesTable(tabuleiro);
    // Checagem na horizontal
    for (int x = 1; x < COLUNAS - 1; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            if (tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x-1][y] && 
                tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x+1][y])
            {
                int i, peça = tabuleiro->tabuleiro[x][y];
                for(i = 1; tabuleiro->tabuleiro[x+i+1][y]/*checa próxima peça*/ == peça; i++); // Define i como índice da maior peça
                while(tabuleiro->tabuleiro[x+i][y] == peça) {
                    tabuleiro->tabuleiro[x+i][y] = 0;
                    i--;
                } // Seta todas as peças do match como 0
                while(tabuleiro->tabuleiro[x+i+1][y] == 0) {
                    bubbleClear(tabuleiro, x+i+1, y);
                    i++;
                } // Executa a função de atualizar em cada peça 0
            }
        }
    }

    // Checagem na vertical
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 1; y < LINHAS - 1; y++)
        {
            if (tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x][y-1] && 
                tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x][y+1])
            {
                int i, peça = tabuleiro->tabuleiro[x][y];
                for(i = 1; tabuleiro->tabuleiro[x][y+i+1]/*checa próxima peça*/ == peça; i++); // Define i como índice da maior peça
                while(tabuleiro->tabuleiro[x][y+i] == peça) {
                    tabuleiro->tabuleiro[x][y+i] = 0;
                    i--;
                }
                while(tabuleiro->tabuleiro[x][y+i+1] == 0) {
                    bubbleClear(tabuleiro, x, y+i+1);
                    i++;
                }
            }
        }
    }
}