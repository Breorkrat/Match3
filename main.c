#include <raylib.h>
#include <string.h>
#include <time.h> 
#include "include/logic.h"

int main(void)
{
    SetRandomSeed(time(NULL));
    game tabuleiro;
    
    tela = MENU;
    InitWindow(LARGURA, ALTURA, "Match3");
    
    Texture2D imgTitle = LoadTexture("images/Title.png");
    background = LoadTexture("images/background.png");
    SetTargetFPS(60);
    int loop60 = 0, direcao = 1;

    Texture2D bg;

    while (!WindowShouldClose())
    {
        loop60 += (direcao);
        if (loop60 == 60 || loop60 == 0) {
            if (direcao == -1) direcao = 1;
            else direcao = -1;
        }
            

        if (tela == LOGO && loop60 == 60) {
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
                DrawRectanglePro((Rectangle){0, 0, LARGURA, ALTURA}, (Vector2){300, 300}, 80, BLACK);
                DrawLineEx((Vector2){305, 0}, (Vector2){390, ALTURA}, 8, WHITE);
                DrawTextureEx(imgTitle, (Vector2){480-loop60/2, 50-loop60/7}, 350+(loop60/6.0 + 2.8), 1+(loop60/300.0 - 0.3), WHITE);
                drawButton(botãoJogar);
                drawButton(escolherNivel);
                drawButton(botãoSair);
                EndDrawing();
                break;

            case NOVOJOGO:
                inicializarJogo(&tabuleiro);
                tela = JOGO;
                break;

            case JOGO:
                if(IsKeyPressed(KEY_F1)) {
                    modo_edicao = !modo_edicao;
                }
                if(modo_edicao) {
                    int pecaAtual = tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]];
                    if(IsKeyPressed(KEY_Z) && pecaAtual > 0) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]--;
                    if(IsKeyPressed(KEY_X) && pecaAtual < NUMTIPOS) tabuleiro.tabuleiro[tabuleiro.cursor[0]][tabuleiro.cursor[1]]++;

                    if(IsKeyDown(KEY_Q) && tabuleiro.objetivos.objPontos > 0) tabuleiro.objetivos.objPontos -= 100;
                    if(IsKeyDown(KEY_W)) tabuleiro.objetivos.objPontos += 100;

                    if(IsKeyDown(KEY_A) && tabuleiro.objetivos.maxJogadas > 0) tabuleiro.objetivos.maxJogadas --;
                    if(IsKeyDown(KEY_S)) tabuleiro.objetivos.maxJogadas ++;

                    if(IsKeyPressed(KEY_C) && tabuleiro.objetivos.objPeca > 0) tabuleiro.objetivos.objPeca--;
                    if(IsKeyPressed(KEY_V) && tabuleiro.objetivos.objPeca < NUMTIPOS) tabuleiro.objetivos.objPeca++;

                    if(IsKeyPressed(KEY_D) && tabuleiro.objetivos.objPecaQuant > 0) tabuleiro.objetivos.objPecaQuant--;
                    if(IsKeyPressed(KEY_F)) tabuleiro.objetivos.objPecaQuant++;

                    if(IsKeyPressed(KEY_F2)) {
                        FILE* save = fopen("saves/save.m3s", "wb");
                        if (save == NULL) return 2;
                        fwrite(&tabuleiro, sizeof(tabuleiro), 1, save);
                        fclose(save);
                    }
                }

                if(IsKeyPressed(KEY_ENTER)) tela = PAUSE;
                tabuleiro.cursor[0] = (GetMouseX()-(INICIO_MATRIZ - RAIO))/LADO;
                tabuleiro.cursor[1] = (GetMouseY()-ALTURA_HUD)/LADO;
                if (!modo_edicao) updateMatches(&tabuleiro);
                draw(&tabuleiro);

                if(tabuleiro.objetivos.maxJogadas > 0 &&
                    tabuleiro.objetivos.maxJogadas - tabuleiro.movimentos <= 0) {
                    tela = LOSE;
                    bg = screenshot();
                }
                if(tabuleiro.objetivos.objPontos > 0 &&
                tabuleiro.pontos >= tabuleiro.objetivos.objPontos) {
                    tela = WIN;
                    bg = screenshot();
                }
            
                if(tabuleiro.objetivos.objPeca > 0 &&
                tabuleiro.objetivos.objPecaQuant > 0 &&
                tabuleiro.pecas[tabuleiro.objetivos.objPeca] > tabuleiro.objetivos.objPecaQuant) {
                    tela = WIN; 
                    bg = screenshot();
                }
                                    
                break;

            case NIVEIS:
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 100});
                DrawText("Arraste o arquivo de jogo aqui", LARGURA/2-500, 40, 30, WHITE);
                drawButton(botãoMenu);

                if (IsFileDropped()){
                    FilePathList listaArquivos = LoadDroppedFiles();
                    char nomeArquivo[100];
                    TextCopy(nomeArquivo, listaArquivos.paths[0]);
                    FILE *nivel = fopen(nomeArquivo, "rb");
                    if (nivel == NULL) return 3;
                    fread(&tabuleiro, sizeof(tabuleiro), 1, nivel);
                    fclose(nivel);
                    UnloadDroppedFiles(listaArquivos);
                    tela = JOGO;
                }
                EndDrawing();
                break;

            case PAUSE:
                BeginDrawing();
                DrawTexture(background, 0, 0, WHITE);
                DrawRectangle(0, 0, LARGURA, ALTURA, (Color){0, 0, 0, 100});
                DrawText("PAUSE", LARGURA/2-360, 40, 65, WHITE);
                drawButton(botãoContinuar);
                drawButton(botãoMenu);
                EndDrawing();
                break;
            
            case WIN:
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(bg, 0, 0, WHITE);
                DrawRectangle(MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM, (Color){0, 0, 0, 200});
                DrawRectangleGradientV(MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM, BLACK, (Color){16, 88, 00, 150});
                DrawRectangleLinesEx((Rectangle){MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM}, 4, WHITE);
                DrawText("Vitória!", LARGURA/2-115, 40, 65, VERDE);

                DrawRectangleLinesEx((Rectangle){LARGURA-MARGEM_JANELA_LARGURA-30, ALTURA_HUD+20, MARGEM_JANELA_LARGURA-20, ALTURA-150}, 2, WHITE);
                DrawText("Highscores", LARGURA-MARGEM_JANELA_LARGURA+45, ALTURA_HUD+30, 25, WHITE);

                DrawText("Sua pontuação:", LARGURA-MARGEM_JANELA_LARGURA-5, ALTURA-ALTURA_HUD, 20, WHITE);
                drawButton(botãoNovoJogo);      
                drawButton(botãoFimMenu);
                EndDrawing();
                break;

            case LOSE:
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(bg, 0, 0, WHITE);
                DrawRectangle(MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM, (Color){30, 0, 0, 200});
                DrawRectangleGradientV(MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM, BLACK, (Color){85, 14, 0, 100});
                DrawRectangleLinesEx((Rectangle){MARGEM, MARGEM, LARGURA-2*MARGEM, ALTURA-2*MARGEM}, 4, WHITE);
                DrawText("Derrota!", LARGURA/2-138, 40, 65, VERMELHO);

                DrawRectangleLinesEx((Rectangle){LARGURA-MARGEM_JANELA_LARGURA-30, ALTURA_HUD+20, MARGEM_JANELA_LARGURA-20, ALTURA-150}, 2, WHITE);
                DrawText("Highscores", LARGURA-MARGEM_JANELA_LARGURA+45, ALTURA_HUD+30, 25, WHITE);

                DrawText("Sua pontuação:", LARGURA-MARGEM_JANELA_LARGURA-5, ALTURA-ALTURA_HUD, 20, WHITE);
                drawButton(botãoNovoJogo);      
                drawButton(botãoFimMenu);
                EndDrawing();
                break;

            case EXIT:
                return 0;
        }
    }
    CloseWindow();
    return 0;
}
