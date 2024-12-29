#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"

typedef enum Telas { LOGO = 0, MENU, JOGO } Telas;

int main(void)
{
    SetRandomSeed(time(NULL));
    game tabuleiro;
    
    inicializarJogo(&tabuleiro);
    Telas tela = LOGO;
    InitWindow(LARGURA, ALTURA, "Match3");
    
    background = LoadTexture("images/background.png");
    SetTargetFPS(60);
    int timerMenu = 0;

    while (!WindowShouldClose())
    {
        if (timerMenu < 60) {
            timerMenu++;
        } else if (tela == LOGO) {
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
                DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 100});
                DrawText("Match 3", LARGURA/2-360, 40, 65, WHITE);

                Rectangle botãoJogar = {LARGURA/2-330, 160, 180, 80};
                DrawRectangle(LARGURA/2-330, 160, 180, 80, BLACK);
                DrawText("Jogar", LARGURA/2-300, 180, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoJogar)) {
                    DrawRectangleLinesEx(botãoJogar, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        tela = JOGO;
                } else DrawRectangleLinesEx(botãoJogar, 2, LIGHTGRAY);

                Rectangle botãoSair = {LARGURA/2-330, 410, 180, 80};
                DrawRectangle(LARGURA/2-330, 410, 180, 80, BLACK);
                DrawText("Sair", LARGURA/2-280, 430, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoSair)) {
                    DrawRectangleLinesEx(botãoSair, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        return 0;
                } else DrawRectangleLinesEx(botãoSair, 2, LIGHTGRAY);

                EndDrawing();
                break;

            case JOGO:
                if(IsKeyPressed(KEY_ENTER)) tela = MENU;
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
