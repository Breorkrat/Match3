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
#define MARGEM_JANELA_LARGURA 300
#define MARGEM_JANELA_ALTURA 20
#define ALTURA_HUD 85
#define LARGURA COLUNAS*LADO + MARGEM_JANELA_LARGURA
#define ALTURA LINHAS*LADO + MARGEM_JANELA_ALTURA+ALTURA_HUD
#define NUMTIPOS 6
#define MAGNITUDE_PTO_DISPLAY 21
#define MAX_TEXTO_BOTAO 100

char modo_edicao = 0;
const Color HUDCOLOR = {10, 10, 10, 180};
const Color CIANO = {100, 255, 255, 255};
const Color VERDE = {60, 255, 0, 255};
const Color VERMELHO = {255, 0, 30, 255};
const Color CORES[] = {BLANK, RED, GREEN, BLUE, YELLOW, PURPLE, CIANO};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));
Texture2D background;
typedef enum Telas { LOGO = 0, MENU, NOVOJOGO, JOGO, NIVEIS, PAUSE, WIN, LOSE, EXIT} Telas;

typedef struct objectives {
    int maxJogadas;                 // Máximo de jogadas do nível
    int objPontos;                  // Objetivo de pontuação do nível
    int objPeca;                    // Peça escolhida como objetivo do nível
    int objPecaQuant;               // Quantidade de peças do objetivo a serem quebradas
} objectives;

typedef struct game {
    int tabuleiro[COLUNAS][LINHAS]; // Valores das peças
    int cursor[2];                  // Posição do cursor em células
    char selecionado;               // Cursor está selecionando? 1 ou 0
    int selecao[2];                 // Posição da célula celecionada em células
    int movimentos;                 // Contador de movimentos
    int pontos;                     // Contador de pontos
    int pecas[NUMTIPOS+1];          // Contador de quantas peças de tipo I foram limpas
    objectives objetivos;
} game;

typedef struct Button {
    Rectangle ret;
    char texto[MAX_TEXTO_BOTAO];
    int txtX;
    int txtY;
    int tamTexto;
    Telas redirect;
} Button;

Telas tela;

Button botãoJogar = {
    (Rectangle){LARGURA / 2 - 400, 160, 310, 80},
    "Novo jogo",
    LARGURA / 2 - 350,
    180,
    40,
    NOVOJOGO
    };

Button botãoSair = {
    (Rectangle){LARGURA/2-330, 410, 180, 80},
    "Sair",
    LARGURA/2-280,
    430,
    40,
    EXIT
};

Button escolherNivel = {
    (Rectangle){LARGURA / 2 - 400, 285, 310, 80},
    "Escolher nível",
    LARGURA/2-390,
    305,
    40,
    NIVEIS
};

Button botãoMenu = {
    (Rectangle){LARGURA/2-400, 285, 310, 80},
    "Menu inicial",
    LARGURA/2-360,
    305,
    40,
    MENU
};

Button botãoFimMenu = {
    (Rectangle){MARGEM + 10, ALTURA_HUD + MARGEM * 8, 250, 40},
    "Voltar ao menu",
    MARGEM+20,
    ALTURA_HUD+MARGEM*8+4,
    30,
    MENU
};

Button botãoNovoJogo = {
    (Rectangle){MARGEM + 10, ALTURA_HUD + MARGEM * 6, 175, 40},
    "Novo jogo",
    MARGEM+20,
    ALTURA_HUD+MARGEM*6+4,
    30,
    NOVOJOGO
};

Button botãoContinuar = {
    (Rectangle){LARGURA/2-400, 160, 310, 80},
    "Continuar",
    LARGURA/2-345,
    180,
    40,
    JOGO
};




void inicializarMatriz(game* tabuleiro) {
    // Cria as peças
    for (int x = 0; x < COLUNAS; x++)
    {
        for (int y = 0; y < LINHAS; y++)
        {
            tabuleiro->tabuleiro[x][y] = GetRandomValue(1, NUMTIPOS);
        }
    }
}

void inicializarJogo(game* tabuleiro)
{
    inicializarMatriz(tabuleiro);
    tabuleiro->movimentos = 0;
    tabuleiro->pontos = 0;
    tabuleiro->selecionado = 0;
    for (int i = 0; i < NUMTIPOS; i++) tabuleiro->pecas[i] = 0;
    tabuleiro->cursor[0] = 0;
    tabuleiro->cursor[1] = 1;
    tabuleiro->objetivos.maxJogadas = 0;
    tabuleiro->objetivos.objPontos = 100;
    tabuleiro->objetivos.objPeca = 0;
    tabuleiro->objetivos.objPecaQuant = 0;
}

// Troca a peça na posição do cursor com uma peça na posição final posição
void swapCells(game* tabuleiro)
{
    int buffer = tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]];

    tabuleiro->tabuleiro[tabuleiro->cursor[0]][tabuleiro->cursor[1]] = tabuleiro->tabuleiro[tabuleiro->selecao[0]][tabuleiro->selecao[1]];
    tabuleiro->tabuleiro[tabuleiro->selecao[0]][tabuleiro->selecao[1]] = buffer;
    tabuleiro->selecionado = 0;
}

int checarVazio(game tabuleiro){
    for (int x = 0; x < COLUNAS; x++)
        for (int y = 0; y < LINHAS; y++)
            if (tabuleiro.tabuleiro[x][y] == 0) return 1;
    return 0;
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

// Valida uma troca de peças
int testSwap(game* tabuleiro){
    tabuleiro->selecionado = 0;

    // Checa se o cursor está dentro do tabuleiro
    if(tabuleiro->cursor[0] >= COLUNAS || tabuleiro->cursor[0] < 0 ||
       tabuleiro->cursor[1] >= LINHAS || tabuleiro->cursor[1] < 0) return 0;
    
    // Checa se o cursor está adjacente à peça selecionada. No modo de edição, qualquer troca é válida
    if (!modo_edicao &&
        abs(tabuleiro->selecao[0] - tabuleiro->cursor[0]) +
        abs(tabuleiro->selecao[1] - tabuleiro->cursor[1]) > 1) return 0;

    // Passa uma cópia do tabuleiro para swapCells para criar o caso do movimento ser executado
    game tempTab = *tabuleiro;
    swapCells(&tempTab);
    if(matchesValidos(tempTab) || modo_edicao) {
        swapCells(tabuleiro);
        if(!modo_edicao) tabuleiro->movimentos++;
        return 1;
    }
    else return 0;
}

void drawHud(game tabuleiro){
    DrawRectangle(0, 0, LARGURA, ALTURA_HUD, HUDCOLOR);
    char temp[MAGNITUDE_PTO_DISPLAY];
    if (tabuleiro.objetivos.maxJogadas <= 0){
        DrawText("Movimentos: ", 5, 9, 20, GREEN);
        sprintf(temp, "%d", tabuleiro.movimentos);
        DrawText(temp, 130, 10, 20, GREEN);
    }
    else {
        DrawText("Movimentos\nrestantes: ", 5, 9, 20, GREEN);
        sprintf(temp, "%d", tabuleiro.objetivos.maxJogadas - tabuleiro.movimentos);
        DrawText(temp, 130, 30, 20, GREEN);
    }

    DrawText("Pontuação: ", 5, 60, 20, GREEN);
    sprintf(temp, "%d", tabuleiro.pontos);
    DrawText(temp, 130, 60, 20, GREEN);

    for(int i = 1; i <= NUMTIPOS; i++){
        sprintf(temp, "%d", tabuleiro.pecas[i]);
        DrawText(temp, 125+i*LADO*1.9, 28, 20, WHITE);
        DrawCircle(170+i*LADO*1.9, 35, RAIO, CORES[i]);
    }
}

// Desenha a grade com o cursor na posição x y
void drawGrid(game* tabuleiro)
{
    DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 210});
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
    DrawTexture(background, 0, 0, WHITE);
    drawGrid(tabuleiro);
    drawHud(*tabuleiro);
    if(modo_edicao) {
        char temp[MAGNITUDE_PTO_DISPLAY];
        DrawRectangleLinesEx((Rectangle){(MARGEM_JANELA_LARGURA/2), ALTURA_HUD, COLUNAS*LADO, LINHAS*LADO}, 3, YELLOW);
        DrawText("Modo de Edição\nAperte F1 para sair", 5, ALTURA_HUD + 15, 15, WHITE);
        DrawText("Objetivo atual de\npontos:", 5, ALTURA_HUD + 65, 15, WHITE);
        sprintf(temp, "%d", tabuleiro->objetivos.objPontos);
        DrawText(temp, 65, ALTURA_HUD + 82, 15, WHITE);

        DrawText("Limite atual de\nmovimentos:", 5, ALTURA_HUD + 115, 15, WHITE);
        sprintf(temp, "%d", tabuleiro->objetivos.maxJogadas);
        DrawText(temp, 95, ALTURA_HUD + 133, 15, WHITE);

        DrawText("Peça do\nobjetivo:", 5, ALTURA_HUD + 175, 15, WHITE);
        DrawCircle(85, ALTURA_HUD + 190, 2*RAIO/3, CORES[tabuleiro->objetivos.objPeca]);

        DrawText("Quantidade:", 5, ALTURA_HUD + 225, 15, WHITE);
        sprintf(temp, "%d", tabuleiro->objetivos.objPecaQuant);
        DrawText(temp, 95, ALTURA_HUD + 224, 15, WHITE);

        DrawText("Z e X para\ntrocar as\npeças\n\n"
        "A e S para\nmodificar limite\nde pontos\n\n"
        "Q e W para alterar\no limite de\nmovimentos\n\n"
        "C e V para alterar\na peça do\nobjetivo\n\n"
        "D e F para alterar\na quantidade\npara objetivo\nde peças\n\n"
        "F2 para salvar\no nível", MARGEM_JANELA_LARGURA/2 + COLUNAS*LADO + 10, ALTURA_HUD + 5, 15, WHITE);
     }
    
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
                for(i = 1; x+i+1 < COLUNAS && tabuleiro->tabuleiro[x+i+1][y] == peça; i++); // Define i como índice da maior peça do match
                int end = i;
                while(x+i >= 0 && tabuleiro->tabuleiro[x+i][y] == peça) i--; // Define i como índice da menor peça do match
                int min = i;

                // Desenha um retângulo verde indicando o match
                BeginDrawing();
                DrawTexture(background, 0, 0, WHITE);
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
                    if (!modo_edicao && tabuleiro->movimentos > 0) tabuleiro->pecas[peça]++;
                    bubbleClear(tabuleiro, x+i+1, y);
                    i++;
                }
                if (!modo_edicao && tabuleiro->movimentos > 0) tabuleiro->pontos += (i-min)*100*(i-min-2);
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
                for(i = 1; y+i+1 < LINHAS && tabuleiro->tabuleiro[x][y+i+1] == peça; i++); // Define i como índice da maior peça do match
                int end = i;
                while(y+i >= 0 && tabuleiro->tabuleiro[x][y+i] == peça) i--; // Define i como índice da menor peça do match
                int min = i;

                // Desenha um retângulo verde indicando o match
                BeginDrawing();
                DrawTexture(background, 0, 0, WHITE);
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
                DrawTexture(background, 0, 0, WHITE);
                drawGrid(tabuleiro);
                EndDrawing();

                // Depois as atualiza
                while(i < end) {
                    if (!modo_edicao && tabuleiro->movimentos > 0) tabuleiro->pecas[peça]++;
                    bubbleClear(tabuleiro, x, y+i+1);
                    i++;
                }
                if (!modo_edicao && tabuleiro->movimentos > 0) tabuleiro->pontos += (i-min)*100*(i-min-2);
            }
        }
    }
}

Texture2D screenshot(){
    EndDrawing();   // Atualiza para que ambas imagens do double buffer
    EndDrawing();   // fiquem com o último estado do tabuleiro
    return LoadTextureFromImage(LoadImageFromScreen());
}

void drawButton(Button botão)
{
    DrawRectangle(botão.ret.x, botão.ret.y, botão.ret.width, botão.ret.height, BLACK);
    DrawRectangleLinesEx(botão.ret, 2, GRAY);
    DrawText(botão.texto, botão.txtX, botão.txtY, botão.tamTexto, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), botão.ret))
    {
        DrawRectangleLinesEx(botão.ret, 3, WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            tela = botão.redirect;
    }
    else
        DrawRectangleLinesEx(botão.ret, 2, LIGHTGRAY);
}