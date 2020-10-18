#include "raylib.h"
#include "Snake.h"
#include "Board.h"
#include <iostream>
#include <cmath>
#include "raymath.h"
#include <fstream>
#include <algorithm>
#include "GameInfoStructs.h"
#include "DrawManager.h"
using namespace std;


void keyInput(Snake & s);
void checkSnakeMovement(Snake & s); // Comprueba al final de cada movimiento si hay comida o impacto
void updateGame(Snake & s);
void relocate(Snake& s);
void initGameVariables(Texture2D * rlLogo, Font * customFont, Snake & s);
void loadHighScores();
void writeScore();
const int screenWidth = 800;
const int screenHeight = 900;
const int NAMEMAXCHAR = 3;

const string scoresFileName = "highScores.dat";
GameInfo gI;

int main(int argc, char* argv[])
{
    //srand((unsigned)time(0));
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Snake!");
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    // TODO: inicializar todas estas variables en una funcion
    Texture2D  rayLibLogoTexture;
    Font customFont;
    Snake s;

    loadHighScores();
    initGameVariables(&rayLibLogoTexture, & customFont, s);

    Board::InitBoard(&s);
    Board::LoadBoards();

    DrawManager::InitDrawManager(&s, &customFont, &gI, &rayLibLogoTexture);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Gestion de la pantalla en la que nos encotramos.
        switch (gI.gameState) {
        case GameStates::logoScreen: // Anim logo intro
            DrawManager::DrawIntroLogo();
            break;

        case GameStates::titleScreen: // Start Menu
            keyInput(s);
            DrawManager::DrawStartMenu();
            break;

        case GameStates::gameScreen : // Game
            keyInput(s);
            updateGame(s);
            DrawManager::DrawPoints();
            s.Draw();
            Board::Draw();
            break;

        case GameStates::gamePause: // GamePaused
            keyInput(s);
            DrawManager::DrawPoints();
            s.Draw();
            Board::Draw();
            DrawManager::DrawPauseMenu();
            break;

        case GameStates::gameOver: // End Game
            keyInput(s);
            DrawManager::DrawEndGameMenu();
            break;

        case GameStates::gameReset: // Reset
            initGameVariables(&rayLibLogoTexture, &customFont, s);
            gI.gameState = GameStates::logoScreen;
            break;

        case GameStates::levelSelectionScreen: // level selection
            DrawManager::DrawLevelSelectionMenu();
            break;
        }
       
        // Incrementamos el tiempo.
        DrawManager::animationTime += GetFrameTime();
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(rayLibLogoTexture);
    UnloadFont(customFont);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


/*
    Gestionamos los inputs del jugador en funcion del estado en el
    que nos encontremos.
*/
void keyInput(Snake & s) { 
    auto & gameState = gI.gameState;

    if (gameState == GameStates::titleScreen) {
        if (IsKeyPressed(KEY_SPACE)) {
            DrawManager::animationTime = 0;
            gameState = GameStates::levelSelectionScreen;
        }
    }
    else if (gameState == GameStates::gameScreen) {
        if (IsKeyPressed(KEY_W)) {
            s.GoUp();
        }
        else if (IsKeyPressed(KEY_S)) {
            s.GoDown();
        }
        else if (IsKeyPressed(KEY_A)) {
            s.GoLeft();
        }
        else if (IsKeyPressed(KEY_D)) {
            s.GoRight();
        } 
        else if (IsKeyPressed(KEY_SPACE)) {
            gameState = GameStates::gamePause;
        }
    }
    else if (gameState == GameStates::gamePause) {
        if (IsKeyPressed(KEY_SPACE)) {
            gameState = GameStates::gameScreen;
        }
    }
    else if (gameState == GameStates::gameOver) {
        int key = GetKeyPressed();
        // Si pulsamos retroceso borramos un caracter.
        if (IsKeyPressed(KEY_BACKSPACE)) {
            gI.playerName.pop_back();
        }
        // Comprobamos que caracteres puede introducir el jugador.
        else if (gI.playerName.length() < NAMEMAXCHAR && (key >= 32) && (key <= 125) && (key != KEY_SPACE)) {
            gI.playerName += char(key);
        }
        /*
            Comprobamos que se ha introducido un nombre para el jugador, lo guardamos
            y reiniciamos el juego.
        */ 
        else if (IsKeyPressed(KEY_ENTER) && gI.playerName.length() == NAMEMAXCHAR) {
            writeScore();
            DrawManager::animationTime = 0;
            gameState = GameStates::gameReset;
        }
    }
    
}


/*
    Una vez la serpiente se come un snack generamos un portal
    que la mueve a la localizacion del otro.
*/
void teleport(Snake& s) {

    Vector2& snakeHead = s.bodyList.front();
    // Marcamos la posicion del primer snack que nos hemos comido como limpio.
    Board::board[(int)snakeHead.x][(int)snakeHead.y] = Board::Empty;
    // Si el snack que nos hemos comido es el snack1 nos movemos al 2 y viceversa.
    if (snakeHead.x == Board::snak1.x && snakeHead.y == Board::snak1.y) {
        snakeHead.x = Board::snak2.x;
        snakeHead.y = Board::snak2.y;
        Board::board[(int)snakeHead.x][(int)snakeHead.y] = Board::Empty;
    }
    else {
        snakeHead.x = Board::snak1.x;
        snakeHead.y = Board::snak1.y;
        Board::board[(int)snakeHead.x][(int)snakeHead.y] = Board::Empty;
    }
}

/*
    Comprobamos si la serpiente interactua con algun objeto
    o si choca consigo misma.
*/
void checkSnakeMovement(Snake & s) { // Comida obstaculo serpiente
    Vector2 snakeHead = s.bodyList.front();
    int & points = gI.points;
    auto & gameState = gI.gameState;
    // Comprobamos si hay comida en la casilla.
    if (Board::board[(int)snakeHead.x][(int)snakeHead.y] == Board::Food) { 
        points++;
        // Movemos la cabeza de la serpiente a la localizacion del otro snack.
        teleport(s);
        // Anyadimos un snack a la lista de comida de la serpiente.
        s.Eat();
        // Volvemos a anayadir comida al tablero.
        Board::PlaceFood();
    }
    else if (Board::board[(int)snakeHead.x][(int)snakeHead.y] == Board::Obstacle || s.CheckCollision()) { // Comprobamos el impacto
        loadHighScores(); // Cargamos las puntuaciones y nos preparamos para mostrar la pantalla de fin
        gameState = GameStates::gameOver;
    }
}

/*
    Actualizamos el juego cada 0.5s
*/
void updateGame(Snake & s) {
    static float frameTime = 0;
    // Incrementamos el tiempo.
    frameTime += GetFrameTime();
    // Cada 0.5s actualizamos el juego y reseteamos el tiempo.
    if (frameTime > 0.5) {
            s.Move();
            // Comprobamos si la serpiente se ha salido de los limites del tablero.
            relocate(s);
            s.UpdateLastDirection();
            // Comprobamos si el movimiento de la serpiente interactua con algun objeto.
            checkSnakeMovement(s);
            frameTime = 0;
    }
}

/*
    Comprobamos si la serpiente sigue dentro de los limites
    de la partida en caso contrario la volvemos a meter.
*/
void relocate(Snake & s) {
    if (s.bodyList.front().x == -1) {
        s.bodyList.front().x = Board::TAM-1;
    }
    else if (s.bodyList.front().x == Board::TAM) {
        s.bodyList.front().x = 0;
    }
    else if (s.bodyList.front().y == -1) {
        s.bodyList.front().y = Board::TAM-1;
    }
    else if (s.bodyList.front().y == Board::TAM) {
        s.bodyList.front().y = 0;
    }
}

void initGameVariables(Texture2D* rlLogo, Font* customFont, Snake& s) {
    *rlLogo = LoadTexture("raylib_logo.png");
    *customFont = LoadFont("mecha.png");
    gI.gameState = GameStates::logoScreen;
    gI.points = 0;
    gI.highScoresList.clear();
    gI.playerName = "";
    loadHighScores();
    s = Snake(9, 9);
}

void loadHighScores() {
    fstream file;
    file.open(scoresFileName, ios::in);
    string textLine;
    while (getline(file, textLine)) {
        string subStringName, subStringScore;
        subStringName = textLine.substr(0, textLine.find(","));
        subStringScore = textLine.substr(textLine.find(",")+1, textLine.size());
        gI.highScoresList.push_back(Score{ subStringName, stoi(subStringScore) });
    }

    sort(gI.highScoresList.begin(), gI.highScoresList.end(), [](Score a, Score b) {return a.score > b.score; });
    file.close();
}

void writeScore() {
    Score score{ gI.playerName, gI.points };
    fstream file;
    file.open(scoresFileName, ios::app);
    string newScore = score.playerName + "," + to_string(score.score) + "\n";
    file << newScore;
    file.close();
}