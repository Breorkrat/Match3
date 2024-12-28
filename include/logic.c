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
#define ALTURA_HUD 85
#define NUMTIPOS 6
#define MAGNITUDE_PTO_DISPLAY 1

const Color HUDCOLOR = {19, 19, 19, 255};
const Color CIANO = {100, 255, 255, 255};
const Color CORES[] = {BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, CIANO};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));
char selecionado = 0;

typedef struct game {
    int tabuleiro[COLUNAS][LINHAS];
    int cursor[2];
    int movimentos;
    int pontos;
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
void swapCells(game* tabuleiro, int final[2])
{
    int buffer = tabuleiro->tabuleiro[final[0]][final[1]];

    tabuleiro->tabuleiro[final[0]][final[1]] = tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]];
    tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]] = buffer;
    selecionado = 0;
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

// Verifica se um dado movimento pode ser executado, recebendo um tabuleiro, as coordenadas para onde a peça
// deve ser movida e se o cursor está em modo de movimento. Caso validado, o movimento é executado
int testSwap(game* tabuleiro, int final[2], char selecionado){
    if(!selecionado) return 0;
    // Passa uma cópia do tabuleiro para swapCells para criar o caso do movimento ser executado
    game tempTab = *tabuleiro;
    swapCells(&tempTab, final);
    if(matchesValidos(tempTab)) {
        swapCells(tabuleiro, final);
        tabuleiro->movimentos ++;
        return 1;
    }
    else return 0;
}

// Atualiza a posição do cursor
void updatePos(game* tabuleiro, char selecionado)
{
    int old[2] = {tabuleiro->cursor[0], tabuleiro->cursor[1]};
    if (IsKeyPressed(KEY_RIGHT))
    {
        // Versão branchless que não usa if statements, versão com IF comentada
        tabuleiro->cursor[0] += (tabuleiro->cursor[0] < COLUNAS - 1);
        //if (tabuleiro->cursor[0] < COLUNAS - 1) tabuleiro->cursor[0]++;
        testSwap(tabuleiro, old, selecionado);
    }
    else if (IsKeyPressed(KEY_LEFT))
    {
        tabuleiro->cursor[0] -= (tabuleiro->cursor[0] > 0);
        //if (tabuleiro->cursor[0] > 0) tabuleiro->cursor[0]--;
        testSwap(tabuleiro, old, selecionado);
    }
    if (IsKeyPressed(KEY_UP))
    {
        tabuleiro->cursor[1] -= (tabuleiro->cursor[1] > 0);
        //if (tabuleiro->cursor[1] > 0) tabuleiro->cursor[1]--;
        testSwap(tabuleiro, old, selecionado);
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        tabuleiro->cursor[1] += (tabuleiro->cursor[1] < LINHAS - 1);
        //if (tabuleiro->cursor[1] < LINHAS - 1) tabuleiro->cursor[1]++;
        testSwap(tabuleiro, old, selecionado);
    }
   
}

void drawHud(game tabuleiro){
    DrawRectangle(0, 0, LARGURA, ALTURA_HUD, HUDCOLOR);
    DrawText("Movimentos: ", 5, 9, 30, GREEN);
    char temp[MAGNITUDE_PTO_DISPLAY];
    sprintf(temp, "%d", tabuleiro.movimentos);
    DrawText(temp, 190, 10, 30, GREEN);

    DrawText("Pontuação: ", 5, 49, 30, GREEN);
    sprintf(temp, "%d", tabuleiro.pontos);
    DrawText(temp, 180, 50, 30, GREEN);
}

// Desenha a grade com o cursor na posição x y
void drawGrid(game tabuleiro, char selecionado)
{
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            // Desenha cursor
            if (x == tabuleiro.cursor[0] && y == tabuleiro.cursor[1])
            {
                Rectangle cursor = {x * LADO, y * LADO + ALTURA_HUD, LADO, LADO};
                DrawRectangleLinesEx(cursor, 3, (selecionado ? RED : YELLOW));
            }
            // Desenha peças
            int tab = tabuleiro.tabuleiro[x][y];
            Color cor = CORES[tab];
            DrawCircle(MARGEM + x * LADO, MARGEM + y * LADO + ALTURA_HUD, RAIO, cor);
        }
    }
}

void draw(game tabuleiro, char selecionado){
    BeginDrawing();
    ClearBackground(BLACK);

    drawGrid(tabuleiro, selecionado);
    drawHud(tabuleiro);
    

    EndDrawing();
}

// Recebe tabuleiro e a posição de uma peça que deverá ser deletada, então, passa ela para o topo
// da pilha e gera uma nova em seu lugar
void bubbleClear(game* tabuleiro, int x, int y){
    draw(*tabuleiro, false);
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
                drawGrid(*tabuleiro, 0);
                drawHud(*tabuleiro);
                Rectangle cursor = {(x+min+1) * LADO, y * LADO + ALTURA_HUD, LADO*(end-min), LADO};
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
                drawGrid(*tabuleiro, 0);
                drawHud(*tabuleiro);
                Rectangle cursor = {x * LADO, (y+min+1) * LADO + ALTURA_HUD, LADO, LADO*(end-min)};
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
                drawGrid(*tabuleiro, 0);
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