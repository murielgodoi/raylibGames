/**
 * @file hello.c
 * @author Muriel Godoi (muriel@utfpr.edu.br)
 * @brief Hello World in Raylib 5.5
 * @version 0.1
 * @date 2024-11-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Hello World Raylib 5.5");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Loop principal do jogo
    while (!WindowShouldClose())    // Detecta o botão de fechar a janela ou a tecla ESC
    {
        // Atualizar
        //----------------------------------------------------------------------------------
        // TODO: Atualize suas variáveis aqui
        //----------------------------------------------------------------------------------

        // Desenha
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Parabéns! Você criou sua primeiro programa com Raylib!", 120, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Desinicialização
    //--------------------------------------------------------------------------------------
    CloseWindow();         // Fecha a janela e o contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}