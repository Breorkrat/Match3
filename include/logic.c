#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define LINHAS 8
#define COLUNAS 10
#define RAIO 20
#define LADO (2*RAIO+10)
#define MARGEM LADO/2
#define LARGURA COLUNAS*LADO
#define ALTURA LINHAS*LADO
#define ALTURA_HUD 85
#define NUMTIPOS 6
#define MAGNITUDE_PTO_DISPLAY 1
#define MARGEM_JANELA_LARGURA 80
#define MARGEM_JANELA_ALTURA 20

const Color HUDCOLOR = {19, 19, 19, 255};
const Color CIANO = {100, 255, 255, 255};
const Color CORES[] = {BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, CIANO};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));

typedef struct game {
    int tabuleiro[COLUNAS][LINHAS]; // Valores das peças
    int cursor[2];                  // Posição do cursor em células
    char selecionado;               // Cursor está selecionando? 1 ou 0
    int selecao[2];                 // Posição da célula celecionada em células
    int movimentos;                 // Contador de movimentos
    int pontos;                     // Contador de pontos
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

// Troca a peça na posição do cursor com uma peça na posição final posição
void swapCells(game* tabuleiro)
{
    int buffer = tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]];

    tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]] = tabuleiro->tabuleiro[tabuleiro->selecao[0]][tabuleiro->selecao[1]];
    tabuleiro->tabuleiro[tabuleiro->selecao[0]][tabuleiro->selecao[1]] = buffer;
    tabuleiro->selecionado = 0;
}

// Retorna 1 se houverem matches no tabuleiro fornecido, caso contrário, retorna 0
int matchesValidos(game tabuleiro){
    for (int x = 1; x < COLUNAS - 1; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            if (tabuleiro.tabuleiro[x][y] == tabuleiro.tabuleiro[x-1][y] && 
                tabuleiro.tabuleiro[x][y] == tabuleiro.tabuleiro[x+1][y])
            {
                return 1;
            }
        }
    }
    // Checagem na vertical
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 1; y < LINHAS - 1; y++)
        {
            if (tabuleiro.tabuleiro[x][y] == tabuleiro.tabuleiro[x][y-1] && 
                tabuleiro.tabuleiro[x][y] == tabuleiro.tabuleiro[x][y+1])
            {
                return 1;
            }
        }
    }
    // Caso nenhum dos ifs ocorram, retorna 0
    return 0;
}

void drawHud(game tabuleiro){
    DrawRectangle(0, 0, LARGURA+MARGEM_JANELA_LARGURA, ALTURA_HUD, HUDCOLOR);
    DrawText("Movimentos: ", 5, 9, 30, GREEN);
    char temp[MAGNITUDE_PTO_DISPLAY];
    sprintf(temp, "%d", tabuleiro.movimentos);
    DrawText(temp, 190, 10, 30, GREEN);

    DrawText("Pontuação: ", 5, 49, 30, GREEN);
    sprintf(temp, "%d", tabuleiro.pontos);
    DrawText(temp, 180, 50, 30, GREEN);
}

int testSwap(game* tabuleiro){
    tabuleiro->selecionado = 0;
    // Se a diferença da posição das peças for maior que 1, retorna
    if (abs(tabuleiro->selecao[0] - tabuleiro->cursor[0]) +
        abs(tabuleiro->selecao[1] - tabuleiro->cursor[1]) > 1) return 0;

    // Passa uma cópia do tabuleiro para swapCells para criar o caso do movimento ser executado
    game tempTab = *tabuleiro;
    swapCells(&tempTab);
    if(matchesValidos(tempTab)) {
        swapCells(tabuleiro);
        tabuleiro->movimentos++;
        return 1;
    }
    else return 0;
}
// Desenha a grade com o cursor na posição x y
void drawGrid(game* tabuleiro)
{
    char changeSelect = 0;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (tabuleiro->selecionado) testSwap(tabuleiro);
        else tabuleiro->selecionado = 1;
        changeSelect = 1;
    }
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            // Desenha peças
            Color cor = CORES[tabuleiro->tabuleiro[x][y]];
            DrawCircle((MARGEM_JANELA_LARGURA / 2) + MARGEM + x * LADO, MARGEM + y * LADO + ALTURA_HUD, RAIO, cor);

            Rectangle pecaPos = {(MARGEM_JANELA_LARGURA / 2) + x * LADO,  y * LADO + ALTURA_HUD, LADO, LADO};
            if (tabuleiro->cursor[0] == x && tabuleiro->cursor[1] == y) {
                DrawRectangleLinesEx(pecaPos, 3, YELLOW);
                if (changeSelect && tabuleiro->selecionado) {
                     tabuleiro->selecao[0] = x;
                     tabuleiro->selecao[1] = y;
                }
            }
        }
    }
    if(tabuleiro->selecionado) {
        Rectangle sel = {(MARGEM_JANELA_LARGURA/2) + tabuleiro->selecao[0]*LADO+2, tabuleiro->selecao[1]*LADO+2 + ALTURA_HUD, LADO-4, LADO-4};
        DrawRectangleLinesEx(sel, 2, RED);
    }
}

void draw(game* tabuleiro){
    BeginDrawing();
    ClearBackground(BLACK);

    drawGrid(tabuleiro);
    drawHud(*tabuleiro);
    
    EndDrawing();
}

// Recebe tabuleiro e a posição de uma peça que deverá ser deletada, então, passa ela para o topo
// da pilha e gera uma nova em seu lugar
void bubbleClear(game* tabuleiro, int x, int y){
    draw(tabuleiro);
    usleep(100000); // Aguarda 0.1 segundo para melhor visualização do movimento
    int i;
    for (i = y; i > 0; i--){
        tabuleiro->tabuleiro[x][i] = tabuleiro->tabuleiro[x][i-1];
        tabuleiro->tabuleiro[x][i-1] = 0;
    }
    tabuleiro->tabuleiro[x][i] = GetRandomValue(1, NUMTIPOS);
}

// Verifica por matches e os atualiza, assim como os pontos
void updateMatches(game* tabuleiro)
{
    // Checagem na horizontal
    for (int x = 1; x < COLUNAS - 1; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            if (tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x-1][y] && 
                tabuleiro->tabuleiro[x][y] == tabuleiro->tabuleiro[x+1][y])
            {
                int i, peça = tabuleiro->tabuleiro[x][y];
                for(i = 1; x+i+1 < LINHAS && tabuleiro->tabuleiro[x+i+1][y] == peça; i++); // Define i como índice da maior peça do match
                int end = i;
                while(x+i >= 0 && tabuleiro->tabuleiro[x+i][y] == peça) i--; // Define i como índice da menor peça do match
                int min = i;

                // Desenha um retângulo verde indicando o match
                BeginDrawing();
                drawGrid(tabuleiro);
                drawHud(*tabuleiro);
                Rectangle cursor = {(MARGEM_JANELA_LARGURA/2) + (x+min+1) * LADO, y * LADO + ALTURA_HUD, LADO*(end-min), LADO};
                DrawRectangleLinesEx(cursor, 2, GREEN);
                EndDrawing();
                usleep(500000);

                int tmp = i;
                // Primeiro mostra todas as peças limpas
                while(tmp < end) {
                    tabuleiro->tabuleiro[x+tmp+1][y] = 0;
                    tmp++;
                }

                // Depois as atualiza
                while(i < end) {
                    bubbleClear(tabuleiro, x+i+1, y);
                    i++;
                }
                if (tabuleiro->movimentos > 0) tabuleiro->pontos += (i-min)*100*(i-min-2);
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
                for(i = 1; y+i+1 < COLUNAS && tabuleiro->tabuleiro[x][y+i+1] == peça; i++); // Define i como índice da maior peça do match
                int end = i;
                while(y+i >= 0 && tabuleiro->tabuleiro[x][y+i] == peça) i--; // Define i como índice da menor peça do match
                int min = i;

                // Desenha um retângulo verde indicando o match
                BeginDrawing();
                drawGrid(tabuleiro);
                drawHud(*tabuleiro);
                Rectangle cursor = {(MARGEM_JANELA_LARGURA/2) + x * LADO, (y+min+1) * LADO + ALTURA_HUD, LADO, LADO*(end-min)};
                DrawRectangleLinesEx(cursor, 2, GREEN);
                EndDrawing();
                usleep(500000);

                int tmp = i;
                // Primeiro mostra todas as peças limpas
                while(tmp < end) {
                    tabuleiro->tabuleiro[x][y+tmp+1] = 0;
                    tmp++;
                }
                BeginDrawing();
                drawGrid(tabuleiro);
                EndDrawing();

                // Depois as atualiza
                while(i < end) {
                    bubbleClear(tabuleiro, x, y+i+1);
                    i++;
                }
                if (tabuleiro->movimentos > 0) tabuleiro->pontos += (i-min)*100*(i-min-2);
            }
        }
    }
}