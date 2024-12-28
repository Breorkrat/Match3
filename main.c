#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"
     
typedef enum Telas { LOGO = 0, MENU, JOGO } Telas;

int main(void)     
{  
    SetRandomSeed(time(NULL));
    game tabuleiro;
    tabuleiro.movimentos = 0;  
    tabuleiro.pontos = 0;      
    inicializarMatriz(&tabuleiro);     
    tabuleiro.cursor[0] = GetRandomValue(0, COLUNAS-1);
    tabuleiro.cursor[1] = GetRandomValue(0, LINHAS-1);
    Telas tela = LOGO;
    InitWindow(LARGURA, ALTURA+ALTURA_HUD, "Match3");
    SetTargetFPS(60);
    int timerMenu = 0;

    while (!WindowShouldClose())
    {
        if (timerMenu < 210) {
            timerMenu++;
        } else {
            tela = JOGO;
            ClearBackground(BLACK);
        }
        switch (tela) {
            case LOGO:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("CRUSHEST\nCANDY", 30, 40, 50, RED);
                EndDrawing();
                break;
            
            case MENU:
                printf("Não fiz ainda :p\n");

            case JOGO:
                if (IsKeyPressed(KEY_X)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]] --;
                if (IsKeyPressed(KEY_Z)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]] ++;
                if (IsKeyPressed(KEY_ENTER)) selecionado = !selecionado;
                updatePos(&tabuleiro, selecionado);
                updateMatches(&tabuleiro);
                draw(tabuleiro, selecionado);
                break;
        }
        
    }
    CloseWindow();
    return 0;
}