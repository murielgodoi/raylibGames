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
    // Inicialização //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    int ballX = 300;
    int ballY = 300;
    int direcaoBallX = 1;
    int direcaoBallY = 1;
    int speed = 10;
    int raioBall = 20;
    int paddlePos1 = screenHeight / 2;
    int paddlePos2 = screenHeight / 2;
    int paddleSize = 200;
    int paddleSpeed = 10;
    int pontos1 = 0;
    int pontos2 = 0;
    Rectangle paddle1 = {20, paddlePos1 - paddleSize / 2, 40, paddleSize};
    Rectangle paddle2 = {screenWidth - 60, paddlePos2 - paddleSize / 2, 40, paddleSize};

    InitWindow(screenWidth, screenHeight, "UTFPR Pong");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second //--------------------------------------------------------------------------------------
    // Loop principal do jogo
    while (!WindowShouldClose())
    { // Detecta o botão de fechar a janela ou a tecla ESC { // Atualizar //---------------------------------------------------------------------------------- // TODO: Atualize suas variáveis aqui //----------------------------------------------------------------------------------
        // Atualiza os valores
        ballX += (direcaoBallX * speed);
        ballY += (direcaoBallY * speed);

        if (ballX < 0 + raioBall)
        {
            ballX = screenWidth / 2;
            ballY = screenHeight / 2;
            pontos2++;
        }
        if (ballX > screenWidth - raioBall)
        {
            ballX = screenWidth / 2;
            ballY = screenHeight / 2;
            pontos1++;
        }
        if (ballY < 0 + raioBall || ballY > screenHeight - raioBall)
        {
            direcaoBallY *= -1;
        }
        if (IsKeyDown(KEY_DOWN) && paddlePos2 < screenHeight - (paddleSize / 2))
        {
            paddlePos2 += paddleSpeed;
        }
        if (IsKeyDown(KEY_UP) && paddlePos2 > paddleSize / 2)
        {
            paddlePos2 -= paddleSpeed;
        }
        if (IsKeyDown(KEY_S) && paddlePos1 < screenHeight - (paddleSize / 2))
        {
            paddlePos1 += paddleSpeed;
        }
        if (IsKeyDown(KEY_W) && paddlePos1 > paddleSize / 2)
        {
            paddlePos1 -= paddleSpeed;
        }
        
        if (IsKeyDown(KEY_R) )
        {
            ballX = screenWidth / 2;
            ballY = screenHeight / 2;
            pontos1 = 0;
            pontos2 = 0;
        }

        paddle1.y = paddlePos1 - paddleSize / 2;
        paddle2.y = paddlePos2 - paddleSize / 2;
        if (CheckCollisionCircleRec((Vector2){ballX, ballY}, raioBall, paddle1))
        {
            direcaoBallX *= -1;
            ballX += (direcaoBallX * speed);
            ballY += (direcaoBallY * speed);
        }
        if (CheckCollisionCircleRec((Vector2){ballX, ballY}, raioBall, paddle2))
        {
            direcaoBallX *= -1;
            ballX += (direcaoBallX * speed);
            ballY += (direcaoBallY * speed);
        }

        // Desenha
        //----------------------------------------------------------------------------------
        BeginDrawing();
        // Desenha a saida
        ClearBackground(RAYWHITE);
        DrawRectangle(screenWidth / 2, 0, 4, screenHeight, LIGHTGRAY);
        DrawText(TextFormat("%02d", pontos1), screenWidth * 0.25 - 30, 20, 60, GRAY);
        DrawText(TextFormat("%02d", pontos2), screenWidth * 0.75 - 30, 20, 60, GRAY);
        DrawCircle(ballX, ballY, raioBall, BLACK);
        DrawRectangle(20, paddlePos1 - paddleSize / 2, 40, paddleSize, RED);
        DrawRectangle(screenWidth - 60, paddlePos2 - paddleSize / 2, 40, paddleSize, BLUE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // Desinicialização
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Fecha a janela e o contexto OpenGL
                   //--------------------------------------------------------------------------------------
    return 0;
}