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
    tabuleiro.selecionado = 0; 
    inicializarMatriz(&tabuleiro);     
    //tabuleiro.cursor[0] = GetRandomValue(0, COLUNAS-1);
    //tabuleiro.cursor[1] = GetRandomValue(0, LINHAS-1);
    Telas tela = LOGO;
    InitWindow(LARGURA, ALTURA+ALTURA_HUD, "Match3");
    SetTargetFPS(60);
    int timerMenu = 0;

    while (!WindowShouldClose())
    {
        if (timerMenu < 210) {
            timerMenu++;
        } else {
            ClearBackground(BLACK);
            tela = JOGO;
        }
        switch (tela) {
            case LOGO:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("CRUSHEST\nCANDY", 30, 40, 50, RED);
                EndDrawing();
                break;
            
            case MENU:
                BeginDrawing();
                ClearBackground(BLACK);
                EndDrawing();
                break;

            case JOGO:
                //if (IsKeyPressed(KEY_X)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]] --;
                //if (IsKeyPressed(KEY_Z)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]] ++;
                //if (IsKeyPressed(KEY_ENTER)) selecionado = !selecionado;
                tabuleiro.cursor[0] = GetMouseX()/LADO;
                tabuleiro.cursor[1] = (GetMouseY()-ALTURA_HUD)/LADO;
                updateMatches(&tabuleiro);
                draw(&tabuleiro);
                break;
        }
        
    }
    CloseWindow();
    return 0;
}