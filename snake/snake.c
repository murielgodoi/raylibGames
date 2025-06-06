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

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include "raylib.h"

typedef struct {
    int velX;
    int velY;
    int size;
    int head;
    int tail[100][2];
} Snake;


void snakeDraw(Snake snake, int squareSize){
    //DrawRectangle(snake.posX*squareSize , snake.posY*squareSize, squareSize, squareSize, DARKGREEN);
    for(int i = snake.size-1; i >= 0; i--){
        DrawRectangle(snake.tail[i][0]*squareSize , snake.tail[i][1]*squareSize, squareSize, squareSize, DARKGREEN);
    }//for
}

void snakeEat(Snake snake, int x, int y){
    snake.tail[snake.size][0] = x;
    snake.tail[snake.size][1] = y;
    snake.size++;
}//snake

bool snakeWillEat(Snake snake, int x, int y){

    int nextX = snake.tail[snake.size-1][0] + snake.velX;
    int nextY = snake.tail[snake.size-1][1] + snake.velY;

    return (nextX == x && nextY == y);
}//snake

void snakeWalk(Snake snake){

    snake.tail[snake.size][0] = snake.tail[snake.size-1][0] + snake.velX;
    snake.tail[snake.size][1] = snake.tail[snake.size-1][1] + snake.velY;

    for(int i = snake.size; i >= 1; i--){
        snake.tail[i-1][0] = snake.tail[i][0];
        snake.tail[i-1][1] = snake.tail[i][1];
    }//for

}//snakeWalk

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Inicialização
    //--------------------------------------------------------------------------------------
    srand(time(NULL));
    const int screenWidth = 1024;
    const int screenHeight = 768;

    const int tiles = 64;
    const int squareSize = screenWidth/tiles;

    const int squareWidth = screenWidth/squareSize;
    const int squareHeight = screenHeight/squareSize;

    Snake snake = { 1, 0, 1, 1,{{squareWidth/2,squareHeight/2}}};

    int applePosX = 10;
    int applePosY = 10;


    InitWindow(screenWidth, screenHeight, "Snake Game - under development");

    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Loop principal do jogo
    while (!WindowShouldClose())    // Detecta o botão de fechar a janela ou a tecla ESC
    {
        // Atualizar
        //----------------------------------------------------------------------------------
        // TODO: Atualize suas variáveis aqui
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_UP)){
            snake.velY = -1;
            snake.velX = 0;
        }
        if (IsKeyDown(KEY_DOWN)){
            snake.velY = 1;
            snake.velX = 0;
        }
        if (IsKeyDown(KEY_LEFT)){
            snake.velY = 0;
            snake.velX = -1;
        }
        if (IsKeyDown(KEY_RIGHT)){
            snake.velY = 0;
            snake.velX = 1;
        }

        //Atualiza posicao da cobra
        //snake.posX += snake.velX;
        //snake.posY += snake.velY;
        if(snakeWillEat(snake, applePosX, applePosY)){
            snakeEat(snake, applePosX, applePosY);
            applePosX = rand() % squareWidth;
            applePosY = rand() % squareHeight;
        }//if
        snakeWalk(snake);

        //Verifica se saiu da tela
        // if(snake.posX < 0) snake.posX = 0;
        // if(snake.posY < 0) snake.posY = 0;
        // if(snake.posY > squareHeight-1) snake.posY = squareHeight-1;
        // if(snake.posX > squareWidth-1) snake.posX = squareWidth-1;

        //if (snake.posX == applePosX && snake.posY == applePosY){
        //    applePosX = rand() % squareWidth;
        //    applePosY = rand() % squareHeight;
        //}
    

        // Desenha
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            snakeDraw(snake, squareSize);
            DrawRectangle(applePosX*squareSize , applePosY*squareSize, squareSize, squareSize, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Desinicialização
    //--------------------------------------------------------------------------------------
    CloseWindow();         // Fecha a janela e o contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}