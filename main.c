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
    Telas tela = LOGO;
    InitWindow(LARGURA+MARGEM_JANELA_LARGURA, ALTURA+ALTURA_HUD+MARGEM_JANELA_ALTURA, "Match3");
    SetTargetFPS(60);
    int timerMenu = 0;
    Texture2D background = LoadTexture("./images/background.png");

    while (!WindowShouldClose())
    {
        if (timerMenu < 210) {
            timerMenu++;
        } else {
            ClearBackground(BLACK);
            tela = MENU;
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
                DrawTexture(background, 0, 0, WHITE);
                //DrawText("Match 3");
                EndDrawing();
                break;

            case JOGO:
                tabuleiro.cursor[0] = (GetMouseX()-(MARGEM_JANELA_LARGURA/2))/LADO;
                tabuleiro.cursor[1] = (GetMouseY()-ALTURA_HUD)/LADO;
                if(IsKeyPressed(KEY_Z)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]--;
                if(IsKeyPressed(KEY_X)) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]++;
                updateMatches(&tabuleiro);
                draw(&tabuleiro);
                break;
        }
    }
    CloseWindow();



    return 0;
}