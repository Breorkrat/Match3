#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"

int main(void)
{
    SetRandomSeed(time(NULL));
    game tabuleiro;
    
    Telas tela = LOGO;
    InitWindow(LARGURA, ALTURA, "Match3");
    
    background = LoadTexture("images/background.png");
    SetTargetFPS(60);
    int timerMenu = 0;

    Rectangle botãoJogar = {LARGURA/2-400, 160, 310, 80};
    Rectangle botãoNivel = {LARGURA/2-400, 285, 310, 80};
    Rectangle botãoSair = {LARGURA/2-330, 410, 180, 80};

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

                DrawRectangle(LARGURA/2-400, 160, 310, 80, BLACK);
                DrawText("Novo Jogo", LARGURA/2-350, 180, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoJogar)) {
                    DrawRectangleLinesEx(botãoJogar, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        inicializarJogo(&tabuleiro);
                        tela = JOGO;
                    }
                } else DrawRectangleLinesEx(botãoJogar, 2, LIGHTGRAY);

                DrawRectangle(LARGURA/2-400, 285, 310, 80, BLACK);
                DrawText("Escolher nível", LARGURA/2-390, 305, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoNivel)) {
                    DrawRectangleLinesEx(botãoNivel, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        tela = NIVEIS;
                } else DrawRectangleLinesEx(botãoNivel, 2, LIGHTGRAY);

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
                if(IsKeyPressed(KEY_F1)) {
                    modo_edicao = !modo_edicao;
                }
                int pecaAtual = tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]];
                if(modo_edicao && IsKeyPressed(KEY_Z) && pecaAtual > 0) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]--;
                if(modo_edicao && IsKeyPressed(KEY_X) && pecaAtual < NUMTIPOS) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]++;
                if(modo_edicao && IsKeyPressed(KEY_F2)) {
                    FILE* save = fopen("saves/save.m3s", "wb");
                    if (save == NULL) return 2;
                    fwrite(&tabuleiro, sizeof(tabuleiro), 1, save);
                    fclose(save);
                }
                if(IsKeyPressed(KEY_ENTER)) tela = PAUSE;
                tabuleiro.cursor[0] = (GetMouseX()-(MARGEM_JANELA_LARGURA/2))/LADO;
                tabuleiro.cursor[1] = (GetMouseY()-ALTURA_HUD)/LADO;
                if (!modo_edicao) updateMatches(&tabuleiro);
                draw(&tabuleiro);
                break;

            case NIVEIS:
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 100});
                DrawText("Arraste o arquivo de jogo aqui", LARGURA/2-500, 40, 30, WHITE);

                if (IsFileDropped()){
                    FilePathList listaArquivos = LoadDroppedFiles();
                    char nomeArquivo[100];
                    TextCopy(nomeArquivo, listaArquivos.paths[0]);
                    FILE *nivel = fopen(nomeArquivo, "rb");
                    if (nivel == NULL) return 3;
                    fread(&tabuleiro, sizeof(tabuleiro), 1, nivel);
                    fclose(nivel);
                    tela = JOGO;
                }
                EndDrawing();
                break;

            case PAUSE:
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 100});
                DrawText("PAUSE", LARGURA/2-360, 40, 65, WHITE);

                
                DrawRectangle(LARGURA/2-400, 160, 310, 80, BLACK);
                DrawText("Continuar", LARGURA/2-345, 180, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoJogar)) {
                    DrawRectangleLinesEx(botãoJogar, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        tela = JOGO;
                    }
                } else DrawRectangleLinesEx(botãoJogar, 2, LIGHTGRAY);
                DrawRectangle(LARGURA/2-400, 285, 310, 80, BLACK);
                DrawText("Menu Inicial", LARGURA/2-360, 305, 40, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), botãoNivel)) {
                    DrawRectangleLinesEx(botãoNivel, 4, WHITE);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        tela = MENU;
                } else DrawRectangleLinesEx(botãoNivel, 2, LIGHTGRAY);
                EndDrawing();
                break;
        }
    }
    CloseWindow();
    return 0;
}
