#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "raylib.h"
#include"leaderSystem.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define MAX_FRAME_SPEED     60
#define MIN_FRAME_SPEED     30
#define TARGET_FPS          60

#define QTE_AVIOES 1
#define QTE_DADOS 2
//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { START, RESTART, GAMEPLAY, PAUSE, ENDING, LEADER, TYPE_LEADER, OPENING } GameScreen;


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector2 windowPosition;
    float speed = 2;
    unsigned long long int globalFrameCounter = 0;
    bool drawScenario = true;
    bool drawAriel = true;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Ariel - O aventureiro");

    // Define textura para poder redimensionar
    int gameScreenWidth = 800;
    int gameScreenHeight = 450;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

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
    Texture2D logo = LoadTexture("img/logo.png");  

    Texture2D fundoCeu = LoadTexture("img/background.png");
    Texture2D chao = LoadTexture("img/tile.png");
    Texture2D hit = LoadTexture("img/hit.png");

    Texture2D ariel = LoadTexture("img/ariel.png");
    Rectangle arielFrameRec = { 0.0f, 0.0f, (float)ariel.width/12, (float)ariel.height };
    Vector2 arielPosicao = { 150.0f, screenHeight - ariel.height - chao.height + 5};
    Rectangle arielArea;

    Texture2D nuvens[5];
    nuvens[0] = LoadTexture("img/cloud1.png");  
    nuvens[1] = LoadTexture("img/cloud2.png");  
    nuvens[2] = LoadTexture("img/cloud3.png");  
    nuvens[3] = LoadTexture("img/cloud4.png");  
    nuvens[4] = LoadTexture("img/cloud5.png");  
    Vector2 nuvemPosicao[5];

    Texture2D montanha = LoadTexture("img/mountain.png");  
    Vector2 montanhasPosicao[4];

    Texture2D aviao = LoadTexture("img/airplane.png");
    Vector2 avioesPosicao[QTE_AVIOES];

    Texture2D arvore;
    arvore = LoadTexture("img/tree2.png");  
    Vector2 arvoresPosicao[4];

    Texture2D dados[QTE_DADOS];
    for (int i = 0; i < QTE_DADOS; i++)
    {
        if(rand() % 2 == 0){
            dados[i] = LoadTexture("img/dice2.png");              
        }else{
            dados[i] = LoadTexture("img/dice3.png");  
        }//else
    }//for
    
    Vector2 dadosPosicao[QTE_DADOS];
    Rectangle areaColisao;

    Leader* leaderBoard = readLeader();

    char name[30] = "\0";
    int letterCount = 0;

    bool colisionDiceStatus[QTE_DADOS];
    for (int i = 0; i < QTE_DADOS; i++){
        colisionDiceStatus[i] = false;
    }//for   

    bool colisionAirplaneStatus[QTE_AVIOES];
    for (int i = 0; i < QTE_AVIOES; i++){
        colisionAirplaneStatus[i] = false;
    }//for
    

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

    GameScreen currentScreen = OPENING;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

// check for alt + enter
 		if (IsKeyPressed(KEY_F11) )
 		{
            // see what display we are on right now
 			int display = GetCurrentMonitor();
 
            
            if (IsWindowFullscreen())
            {
                // if we are full screen, then go back to the windowed size
 			    ToggleFullscreen();
                SetWindowSize(screenWidth, screenHeight);
                SetWindowPosition(windowPosition.x, windowPosition.y);
            }
            else
            {
                // if we are not full screen, set the window size to match the monitor we are on
                windowPosition = GetWindowPosition();
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
 			    ToggleFullscreen();
            }
 
            // toggle the state
 		}



        globalFrameCounter++;

        if(drawScenario){

            //Atualiza scrolling do céu (background)
            scrollingBack -= 0.3f * speed;
            if (scrollingBack <= -fundoCeu.width) scrollingBack = 0;

            //Atualiza posicao das nuvens
            for (int i = 0; i < 5; i++){
                nuvemPosicao[i].x -= 0.5 * speed;
                if(nuvemPosicao[i].x < -nuvens[i].width ) nuvemPosicao[i].x = rand()%screenWidth + screenWidth;
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

        }//if play scenario

        if(drawAriel){
            // Atualiza frame do sprite do Ariel considerando o frameRate
            framesCounter++;
            if (framesCounter >= (TARGET_FPS/(framesSpeed*speed))){
                framesCounter = 0;
                currentFrame++;
                if (currentFrame > 11) currentFrame = 0;

                arielFrameRec.x = (float)currentFrame*(float)ariel.width/12;

                if (jump) currentFrame = 2;

            }//if

        }

        switch(currentScreen){
            
        case OPENING:
            // Keyboard Inputs
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = START;
                PauseMusicStream(music);
                PlaySound(sndPause);
            }   
            break;

        case START:
            pontuacao = 0;
            vidas = 5;

            for (int i = 0; i < 5; i++){
                nuvemPosicao[i].x = (rand()%(screenWidth*2));
                nuvemPosicao[i].y = rand()%150 + 10;
            }//for

            for (int i = 0; i < QTE_DADOS; i++){
                dadosPosicao[i].x = (rand()%(screenWidth*2)) + screenWidth;
                dadosPosicao[i].y = screenHeight - dados[i].height- chao.height + 5;
            }//for 

            for (int i = 0; i < QTE_AVIOES; i++){
                avioesPosicao[i].x = (rand()%(screenWidth*2)) + screenWidth;
                avioesPosicao[i].y = screenHeight - ariel.height - chao.height - aviao.height -  rand() % 200;
            }//for 

            for (int i = 0; i < 4; i++){
                montanhasPosicao[i].x = (rand()%(screenWidth*2));
                montanhasPosicao[i].y = screenHeight - montanha.height- chao.height + rand()% 120;
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
           
            
            //Atualiza posicao dos dados
            for (int i = 0; i < QTE_DADOS; i++){
                dadosPosicao[i].x -= 2 * speed;
                if(dadosPosicao[i].x < -dados[i].width ){
                    if (pontuacao % 1000 > 900){
                        dadosPosicao[i].x = rand()%screenWidth + screenWidth * 0.5 * speed;
                    }else{
                        dadosPosicao[i].x = rand()%screenWidth + screenWidth;
                    }//else
                }//if
            }//for    

            //Atualiza posicao dos avioes
            for (int i = 0; i < QTE_AVIOES; i++){
                avioesPosicao[i].x -= 5 * speed;
                if(avioesPosicao[i].x < - aviao.width ){
                    avioesPosicao[i].y = screenHeight - ariel.height - chao.height - aviao.height -  rand() % 200;
                    if (pontuacao % 1000 > 900){
                        avioesPosicao[i].x = rand()%screenWidth + screenWidth * 0.5 * speed;
                    }else{
                        avioesPosicao[i].x = rand()%screenWidth + screenWidth;
                    }//else
                }//if
            }//for  

            //Verifica colisões
            arielArea = (Rectangle){arielPosicao.x, arielPosicao.y - jumpHeight, ariel.width/12, ariel.height};
            for (int i = 0; i < QTE_DADOS; i++){
                areaColisao = (Rectangle){dadosPosicao[i].x, dadosPosicao[i].y, dados[i].width, dados[i].height};
                if(CheckCollisionRecs(arielArea,areaColisao) && !colisionDiceStatus[i]){
                    vidas--;
                    colisionDiceStatus[i] = true;
                    PlaySound(sndOutch);
                }//if
                if (!CheckCollisionRecs(arielArea,areaColisao) && colisionDiceStatus[i]) colisionDiceStatus[i] = false;
            }//for 

            for (int i = 0; i < QTE_AVIOES; i++){
                areaColisao = (Rectangle){avioesPosicao[i].x, avioesPosicao[i].y, aviao.width, aviao.height};
                if(CheckCollisionRecs(arielArea,areaColisao) && !colisionAirplaneStatus[i]){
                    vidas--;
                    colisionAirplaneStatus[i] = true;
                    PlaySound(sndOutch);
                }//if
                if (!CheckCollisionRecs(arielArea,areaColisao) && colisionAirplaneStatus[i]) colisionAirplaneStatus[i] = false;
            }//for   


            if(vidas <= 0){
                currentScreen = TYPE_LEADER;
                jumpHeight = 0;
                jumpTime = 0;
                jumpStart = 0;
                jump = 0;
                PlaySound(sndGameOver);
            }//if

            // Keyboard Inputs
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = PAUSE;
                PauseMusicStream(music);
                PlaySound(sndPause);
                drawScenario = false;
                drawAriel = false;
            }    

            if (IsKeyPressed(KEY_L)){
                currentScreen = LEADER;
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
                drawScenario = true;
                drawAriel = true;
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

        case LEADER:
            if (IsKeyPressed(KEY_ENTER)){
                currentScreen = OPENING;
                ResumeMusicStream(music);
                PlaySound(sndPause);
            }//if
            break;
        
        case TYPE_LEADER:
            int key = GetCharPressed();
            while (key > 0){
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_LENGTH - 1))
                {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0){
                letterCount--;
                name[letterCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && letterCount >= 1){
                addLeader(leaderBoard,name, (int) pontuacao/10);
                writeLeader(leaderBoard);
                name[0] = '\0';
                letterCount = 0;
                currentScreen = LEADER;
                ResumeMusicStream(music);
                PlaySound(sndPause);
            }//if
            break;
            
        }//switch

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        
        // Desenha na textura para depois poder redimensionar
        BeginTextureMode(target);
        if(drawScenario){            

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

        }//if playScenario

        if(drawAriel){
            //Desenha o ariel
            DrawTextureRec(ariel, arielFrameRec, (Vector2){arielPosicao.x,arielPosicao.y-jumpHeight}, WHITE);  // Draw part of the texture
        }

        switch(currentScreen){

        case OPENING:
        DrawTexture(logo, screenWidth/2-logo.width/2, 80 ,WHITE);  // Draw part of the texture
        DrawText(TextFormat("Pressione ENTER para iniciar..."),screenWidth/2 - MeasureText("Pressione ENTER para iniciar...",20)/2,screenHeight-40,20, WHITE);
            break;
            
        case GAMEPLAY:
        

            //Desenha os dados       
            for (int i = 0; i < QTE_DADOS; i++) {
                DrawTextureEx(dados[i], dadosPosicao[i],0,1,WHITE);  // Draw part of the texture
                if(colisionDiceStatus[i] ){
                    DrawTextureEx(hit,dadosPosicao[i],0,0.1,WHITE);
                }
            }//for         

            //Desenha os avioes       
            for (int i = 0; i < QTE_AVIOES; i++) {
                DrawTextureEx(aviao, avioesPosicao[i],0,1,WHITE);  // Draw part of the texture
                if(colisionAirplaneStatus[i] ){
                     DrawTextureEx(hit,avioesPosicao[i],0,0.1,WHITE);
                }
            }//for 

            
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
        
        case LEADER:

            char textBuffer[12];
            DrawText(TextFormat("Leaderboard"),(screenWidth/2) - MeasureText("Leaderboard",40)/2,screenHeight/8, 40, VIOLET);
            for (int i = 0; i < LEADER_SIZE; i++){
                sprintf(textBuffer,"%02d",i+1);  
                DrawText(TextFormat(textBuffer),170,100+20*i, 20, VIOLET);
                if(leaderBoard[i].name[0] == '\0'){
                    DrawText(TextFormat("---"),200,100+20*i, 20, VIOLET);
                    DrawText(TextFormat("     ---"),500,100+20*i, 20, VIOLET);            
                }else{
                    DrawText(TextFormat(leaderBoard[i].name),200,100+20*i, 20, VIOLET);
                    sprintf(textBuffer,"%8d",leaderBoard[i].score);   
                    DrawText(TextFormat(textBuffer),500,100+20*i, 20, VIOLET);
                }//else
            }//for
            
            DrawText(TextFormat("pressione Enter para continuar..."),(screenWidth/2) - MeasureText("pressione Enter para continuar...",20)/2,screenHeight*0.9, 20, WHITE);
            break;

        case TYPE_LEADER:
            DrawText(TextFormat("FIM DE JOGO"),(screenWidth/2) - MeasureText("FIM DE JOGO!",40)/2,screenHeight/4-20, 40, VIOLET);
            DrawText(TextFormat("Sua pontuação: %d pontos",pontuacao/10),(screenWidth/2) - MeasureText("Sua pontuação: 000 pontos",30)/2,screenHeight/3, 30, VIOLET);
            DrawText(TextFormat("Digite seu nome:"),(screenWidth/2) - MeasureText("Digite seu nome:",20)/2, 250, 20, VIOLET);

            DrawRectangle((screenWidth/2) - 150, 280, 300, 60, GRAY);
            DrawRectangleLines((screenWidth/2) - 150, 280, 300, 60, GREEN);
            DrawText(name, (screenWidth/2) - 130, 300, 40, VIOLET);
            if (letterCount < 30){ // Draw blinking underscore char
                if (globalFrameCounter/20 % 2 == 0)
                    DrawText("_", (screenWidth/2) - 130 + MeasureText(name, 40)+8,  300, 40, VIOLET);
            }//if
            break;

        }//switch
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK); // Limpa o fundo da tela

        // Desenha a textura na tela na escala correta
        DrawTexturePro(target.texture,
                       (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                       (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                       (float)gameScreenWidth*scale, (float)gameScreenHeight*scale },
                       (Vector2){ 0, 0 },
                       0.0f,
                       WHITE);


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