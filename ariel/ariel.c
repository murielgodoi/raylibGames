#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1
#define TARGET_FPS          60
//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { START, RESTART, GAMEPLAY, PAUSE, ENDING } GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    float speed = 2;

    InitWindow(screenWidth, screenHeight, "Ariel em Raylib");

    InitAudioDevice();// Inicializa o dispositivo de audio

    //Carrega a música de fundo
    Music music = LoadMusicStream("sounds/adventuring_song.mp3");
    PlayMusicStream(music);
    bool playMusic = true;

    //Carrega os sons do jogo
    Sound sndJump = LoadSound("sounds/jump.mp3");
    Sound sndOutch = LoadSound("sounds/outch.mp3");
    Sound sndLevelUp = LoadSound("sounds/levelUp.mp3");
    Sound sndPause = LoadSound("sounds/pause.mp3");
    Sound sndGameOver = LoadSound("sounds/gameOver.mp3");

    //Carrega as texturas
    Texture2D fundoCeu = LoadTexture("img/background.png");
    Texture2D chao = LoadTexture("img/tile.png");
    Texture2D hit = LoadTexture("img/hit.png");

    Texture2D ariel = LoadTexture("img/ariel.png");
    Rectangle arielFrameRec = { 0.0f, 0.0f, (float)ariel.width/12, (float)ariel.height };
    Vector2 arielPosicao = { 200.0f, screenHeight - ariel.height - chao.height + 5};
    Rectangle arielArea;

    Texture2D nuvens[5];
    nuvens[0] = LoadTexture("img/cloud1.png");  
    nuvens[1] = LoadTexture("img/cloud2.png");  
    nuvens[2] = LoadTexture("img/cloud3.png");  
    nuvens[3] = LoadTexture("img/cloud4.png");  
    nuvens[4] = LoadTexture("img/cloud5.png");  
    Vector2 nuvemPosicao[5];

    Texture2D montanha;
    montanha = LoadTexture("img/mountain.png");  
    Vector2 montanhasPosicao[4];

    Texture2D arvore;
    arvore = LoadTexture("img/tree2.png");  
    Vector2 arvoresPosicao[4];

    Texture2D dados[2];
    dados[0] = LoadTexture("img/dice2.png");  
    dados[1] = LoadTexture("img/dice3.png");  
    Vector2 dadosPosicao[5];
    Rectangle dadosArea;

    int colisionStatus[2]={0 , 0};

    int jump = 0;
    bool exibeNivel = false;
    int jumpTime = 0;
    int jumpHeight = 0;
    int jumpStart = 0;
    int newJumpHeight = 0;
    int pontuacao = 0;
    int vidas = 5;
    int nivel = 1;    


    float scrollingBack = 0;

    Vector2 positionChao = { 20.0f, screenHeight - chao.height+1};
    float inicioChao = +chao.width;
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 4 * speed;            // Number of spritesheet frames shown by second

    SetTargetFPS(TARGET_FPS);               // Set our game to run at 60 frames-per-second

    GameScreen currentScreen = START;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        switch(currentScreen){

        case START:
            pontuacao = 0;
            vidas = 5;

            for (int i = 0; i < 5; i++){
                nuvemPosicao[i].x = (rand()%(screenWidth*2));
                nuvemPosicao[i].y = rand()%150 + 10;
            }//for

            for (int i = 0; i < 2; i++){
                dadosPosicao[i].x = (rand()%(screenWidth*2)) + screenWidth;
                dadosPosicao[i].y = screenHeight - dados[i].height- chao.height + 5;
            }//for 

            for (int i = 0; i < 4; i++){
                montanhasPosicao[i].x = (rand()%(screenWidth*2));
                montanhasPosicao[i].y = screenHeight - montanha.height- chao.height + rand()%120;
            }//for 

            for (int i = 0; i < 4; i++){
                arvoresPosicao[i].x = (rand()%(screenWidth*2));
                arvoresPosicao[i].y = screenHeight - arvore.height- chao.height + rand()%120;
            }//for 

            currentScreen = GAMEPLAY;   

            StopMusicStream(music);     
            PlayMusicStream(music); 

            break;    
        
        case GAMEPLAY:
            ResumeMusicStream(music);
            if(playMusic) UpdateMusicStream(music);
        
            // Update
            //----------------------------------------------------------------------------------
            //Atualiza pontuação e velocidade proporcional
            pontuacao++;
            speed = (pontuacao/1000)*0.5 + 2.5;
            nivel = pontuacao /1000 + 1;
            
            //Som e mensagem ao passar de nível
            if(pontuacao % 1000 == 0){
                PlaySound(sndLevelUp);
            }//if
            exibeNivel = (pontuacao %1000 < 20) || ((pontuacao %1000 > 30) && (pontuacao %1000 < 100));

            //Atualiza scrolling do céu (background)
            scrollingBack -= 0.3f * speed;
            if (scrollingBack <= -fundoCeu.width) scrollingBack = 0;

            // Atualiza frame do sprite do Ariel considerando o frameRate
            framesCounter++;
            if (framesCounter >= (TARGET_FPS/(framesSpeed*speed))){
                framesCounter = 0;
                currentFrame++;
                if (currentFrame > 11) currentFrame = 0;

                arielFrameRec.x = (float)currentFrame*(float)ariel.width/12;

                if (jump) currentFrame = 2;

            }//if

            //Atualiza posicao das nuvens
            for (int i = 0; i < 5; i++){
                nuvemPosicao[i].x -= 0.5 * speed;
                if(nuvemPosicao[i].x < -nuvens[i].width ) nuvemPosicao[i].x = rand()%screenWidth + screenWidth;
            }//for

            //Atualiza posicao dos dados
            for (int i = 0; i < 2; i++){
                dadosPosicao[i].x -= 2 * speed;
                if(dadosPosicao[i].x < -dados[i].width ){
                    if (pontuacao % 1000 > 900){
                        dadosPosicao[i].x = rand()%screenWidth + screenWidth * 0.5 * speed;
                    }else{
                        dadosPosicao[i].x = rand()%screenWidth + screenWidth;
                    }//else
                }//if
            }//for    

            //Atualiza posicao das montanhas
            for (int i = 0; i < 4; i++){
                montanhasPosicao[i].x -= 0.8 * speed;
                if(montanhasPosicao[i].x < -montanha.width ){                   
                        montanhasPosicao[i].x = rand()%screenWidth + screenWidth;
                        montanhasPosicao[i].y = screenHeight - montanha.height- chao.height + 5 + rand()%120;

                  
                }//if
            }//for  

            //Atualiza posicao das arvores
            for (int i = 0; i < 4; i++){
                arvoresPosicao[i].x -= 2 * speed;
                if(arvoresPosicao[i].x < -arvore.width ){                   
                        arvoresPosicao[i].x = rand()%screenWidth + screenWidth;
                        arvoresPosicao[i].y = screenHeight - arvore.height- chao.height + 5 + rand()%20;                  
                }//if
            }//for     

            // Atualiza scrolling chão        
            inicioChao -= 2*speed;
            if(inicioChao <=  0) inicioChao = chao.width-1;
            
            //Verifica colisões
            arielArea = (Rectangle){arielPosicao.x, arielPosicao.y - jumpHeight, ariel.width/12, ariel.height};
            for (int i = 0; i < 2; i++){
                dadosArea = (Rectangle){dadosPosicao[i].x, dadosPosicao[i].y, dados[i].width, dados[i].height};
                if(CheckCollisionRecs(arielArea,dadosArea) && colisionStatus[i] == 0){
                    vidas--;
                    colisionStatus[i] = 1;
                    PlaySound(sndOutch);
                }//if
                if (!CheckCollisionRecs(arielArea,dadosArea) && colisionStatus[i] ==1) colisionStatus[i] = 0;
            }//for            
            
            if(vidas == 0){
                currentScreen = ENDING;
                PlaySound(sndGameOver);
            }//if

            // Keyboard Inputs
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = PAUSE;
                PauseMusicStream(music);
                PlaySound(sndPause);
            }    

            if ((IsKeyPressed(KEY_M))){
                if(playMusic) ResumeMusicStream(music);
                else PauseMusicStream(music);
                 
                playMusic = !playMusic;
            }//else

            if (IsKeyPressed(KEY_R)){
                currentScreen = RESTART;
                PauseMusicStream(music);
                PlaySound(sndPause);
            }    

            if (IsKeyPressed(KEY_UP)) jump++;
            // Se está pulando, determina a altura do pulo
            if(jump){
                jumpTime++;

                newJumpHeight = jumpStart + (25*jumpTime)-((1.5*pow(jumpTime,2)/2)); //Formula lançamento vertical
                if (newJumpHeight < jumpHeight && IsKeyPressed(KEY_UP) && jump <= 2){//Pulo Duplo
                    jump++;
                    jumpTime = 1;
                    jumpStart = newJumpHeight;
                }
                jumpHeight = newJumpHeight;

                if(jumpTime == 1){ // Inicio do pulo
                    PlaySound(sndJump);
                }//if
                if (jumpHeight < 0){ //Se chegou no chao, finaliza o pulo
                    jumpHeight = 0;
                    jumpTime = 0;
                    jumpStart = 0;
                    jump = 0;
                }//if
            }//if jump
            break;

        case PAUSE:
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = GAMEPLAY;
                ResumeMusicStream(music);
                PlaySound(sndPause);
            }//if
            break;

        case RESTART:
            if (IsKeyPressed(KEY_S)){
                currentScreen = START;
                PlaySound(sndPause);
            }//if
            if (IsKeyPressed(KEY_N)){
                currentScreen = GAMEPLAY;
                ResumeMusicStream(music);
                PlaySound(sndPause);
            }//if
            break;
        
        case ENDING:
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = START;
                ResumeMusicStream(music);
                PlaySound(sndPause);
            }//if
            break;
        }//switch
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        switch(currentScreen){
            
        case GAMEPLAY:
        
            ClearBackground(WHITE);
            //Desenha o céu de fundo
            DrawTexture(fundoCeu, scrollingBack, 0,WHITE);
            DrawTexture(fundoCeu, scrollingBack+ fundoCeu.width, 0,WHITE);

            //Desenha as nuvens       
            for (int i = 0; i < 5; i++) {
                DrawTextureEx(nuvens[i], nuvemPosicao[i],0,0.4,ColorAlpha(WHITE,0.6));  // Draw part of the texture
            }//for    

            //Desenha as montanhas       
            for (int i = 0; i < 4; i++) {
                DrawTextureEx(montanha, montanhasPosicao[i],0,1,WHITE);  // Draw part of the texture
            }//for 

            //Desenha as arvores       
            for (int i = 0; i < 2; i++) {
                DrawTextureEx(arvore, arvoresPosicao[i],0,1,ColorAlpha(WHITE,0.9));  // Draw part of the texture
            }//for 

            //Desenha o chao
            positionChao.x = inicioChao - chao.width;            
            for (int i = 0; i < (screenWidth/chao.width)+3; i++) {
                DrawTexture(chao, positionChao.x, positionChao.y,WHITE);  // Draw part of the texture
                positionChao.x += chao.width-1;
            }//for      

            //Desenha os dados       
            for (int i = 0; i < 2; i++) {
                DrawTextureEx(dados[i], dadosPosicao[i],0,1,WHITE);  // Draw part of the texture
                if(colisionStatus[i] ){
                    DrawTextureEx(hit,dadosPosicao[i],0,0.1,WHITE);
                }
            }//for         

            //Desenha o ariel
            DrawTextureRec(ariel, arielFrameRec, (Vector2){arielPosicao.x,arielPosicao.y-jumpHeight}, WHITE);  // Draw part of the texture

            DrawText(TextFormat("Vidas: %2d",vidas),20,20,20, DARKGREEN);
            DrawText(TextFormat("Pontos: %5d",pontuacao/10),screenWidth - MeasureText("Pontos: 00000",20)-20,20,20, DARKGREEN);

            if(exibeNivel){
                DrawText(TextFormat("Nível %d",nivel),(screenWidth/2) - MeasureText("Nivel 2",30)/2,30, 30, VIOLET);    
            }//if

            break;
        
        case PAUSE:
            DrawText(TextFormat("PAUSE"),(screenWidth/2) - MeasureText("PAUSE",40)/2,screenHeight/2-20, 40, VIOLET);
            break;

        case START:
            break;

        case RESTART:
            DrawText(TextFormat("Deseja reiniciar o jogo? (S/N)"),(screenWidth/2) - MeasureText("Deseja reiniciar o jogo? (S/N)",30)/2,screenHeight/2-15, 30, VIOLET);
            break;
        
        case ENDING:
            DrawText(TextFormat("FIM DE JOGO"),(screenWidth/2) - MeasureText("FIM DE JOGO!",40)/2,screenHeight/4-20, 40, VIOLET);
            DrawText(TextFormat("Sua pontuação: %d pontos",pontuacao/10),(screenWidth/2) - MeasureText("Sua pontuação: 000 pontos",30)/2,screenHeight/3, 30, VIOLET);
            DrawText(TextFormat("pressione Enter para reiniciar..."),(screenWidth/2) - MeasureText("pressione Enter para reiniciar...",20)/2,screenHeight/2, 20, VIOLET);
            break;

        }//switch

        EndDrawing();
    //----------------------------------------------------------------------------------
    }//while main game loop

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(ariel);       // Texture unloading
    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}