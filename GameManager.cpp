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
/*

*/


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

const std::string scoresFileName = "highScores.dat";
GameInfo gI;

int main(int argc, char* argv[])
{
    //srand((unsigned)time(0));
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    loadHighScores();
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    // TODO: inicializar todas estas variables en una funcion
    Texture2D  rayLibLogoTexture;
    Font customFont;
    Snake s;
    
    initGameVariables(&rayLibLogoTexture, & customFont, s);
    Board::InitBoard(&s);
    Board::LoadBoards();
    DrawManager::InitDrawManager(&s, &customFont, &gI, &rayLibLogoTexture);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        static double windowTimeOffset = GetTime(); // TODO: Esto se puede sacar de aqui
        BeginDrawing();
        cout << DrawManager::animationTime << endl;

        ClearBackground(RAYWHITE);
        switch (gI.gameState) {
        case 0: // Anim intro
            DrawManager::DrawIntroLogo();
            break;

        case 1: // Start Menu
            DrawManager::DrawStartMenu();
            keyInput(s);
            break;
        case 2: // Game
            keyInput(s);
            updateGame(s);
            DrawManager::DrawPoints();
            // drawPoints();
            s.Draw();
            Board::Draw();
            break;
        case 3: // GamePaused
            keyInput(s);
            DrawManager::DrawPoints();
            s.Draw();
            Board::Draw();
            DrawManager::DrawPauseMenu();
            break;
        case 4: // End Game
            keyInput(s);
            DrawManager::DrawEndGameMenu();
            break;
        case 5: // Reset
            initGameVariables(&rayLibLogoTexture, &customFont, s);
            gI.gameState = GameStates::logoScreen;
            break;
        case 6: // level selection
            DrawManager::DrawLevelSelectionMenu();
            break;
        }
       
        
        DrawManager::animationTime += GetFrameTime();
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(rayLibLogoTexture);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void keyInput(Snake & s) { // TODO: Mover el init board aqui
    auto & gameState = gI.gameState;
    if (gameState == (int)GameStates::titleScreen) {
        if (IsKeyPressed(KEY_SPACE)) {
            DrawManager::animationTime = 0;
            gameState = GameStates::levelSelectionScreen;
        }
    }
    else if (gameState == (int)GameStates::gameScreen) {
        if (IsKeyPressed(KEY_KP_8)) {
            s.GoUp();
        }
        else if (IsKeyPressed(KEY_KP_2)) {
            s.GoDown();
        }
        else if (IsKeyPressed(KEY_KP_4)) {
            s.GoLeft();
        }
        else if (IsKeyPressed(KEY_KP_6)) {
            s.GoRight();
        } 
        else if (IsKeyPressed(KEY_SPACE)) {
            gameState = GameStates::gamePause;
        }
    }
    else if (gameState == (int)GameStates::gamePause) {
        if (IsKeyPressed(KEY_SPACE)) {
            DrawManager::animationTime = 0;
            gameState = GameStates::gameScreen;
        }
    }
    else if (gameState == (int)GameStates::gameOver) {
        int key = GetKeyPressed();
        std::cout << key << std::endl;
        if (IsKeyPressed(KEY_BACKSPACE)) {
            gI.playerName.pop_back();
        }
        else if (gI.playerName.length() < NAMEMAXCHAR && (key >= 32) && (key <= 125) && (key != KEY_SPACE)) {
            gI.playerName += char(key);
        }
        else if (IsKeyPressed(KEY_ENTER) && gI.playerName.length() == NAMEMAXCHAR) {
            writeScore();
            DrawManager::animationTime = 0;
            gameState = GameStates::gameReset;
        }
    }
    
}

void teleport(Snake& s) {

    Vector2& snakeHead = s.bodyList.front();
    Board::board[(int)snakeHead.x][(int)snakeHead.y] = Board::Empty;
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

void checkSnakeMovement(Snake & s) { // Comida obstaculo serpiente
    Vector2 snakeHead = s.bodyList.front();
    int & points = gI.points;
    auto & gameState = gI.gameState;
    if (Board::board[(int)snakeHead.x][(int)snakeHead.y] == Board::Food) { // Si hay comida se garantiza que la serpiente no ha pasado por ahi
        points++;
        teleport(s);
        s.Eat();
        Board::PlaceFood();
    }
    else if (Board::board[(int)snakeHead.x][(int)snakeHead.y] == Board::Obstacle || s.CheckCollision()) { // Comprobamos el impacto
        loadHighScores(); // Cargamos las puntuaciones y nos preparamos para mostrar la pantalla de fin
        gameState = GameStates::gameOver;
    }
}

void updateGame(Snake & s) {
    static float frameTime = 0;
    static bool isPaused = false;
    frameTime += GetFrameTime();

    if (frameTime > 0.5) {
            s.Move();
            relocate(s);
            s.UpdateLastDirection();
            checkSnakeMovement(s);
            frameTime = 0;
    }
}

void relocate(Snake & s) {
    if (s.bodyList.front().x == -1) {
        s.bodyList.front().x = 15;
    }
    else if (s.bodyList.front().x == 16) {
        s.bodyList.front().x = 0;
    }
    else if (s.bodyList.front().y == -1) {
        s.bodyList.front().y = 15;
    }
    else if (s.bodyList.front().y == 16) {
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
    std::fstream file;
    file.open(scoresFileName, std::ios::in);
    std::string textLine;
    while (std::getline(file, textLine)) {
        std::string subStringName, subStringScore;
        subStringName = textLine.substr(0, textLine.find(","));
        subStringScore = textLine.substr(textLine.find(",")+1, textLine.size());
        gI.highScoresList.push_back(Score{ subStringName, std::stoi(subStringScore) });
    }

    std::sort(gI.highScoresList.begin(), gI.highScoresList.end(), [](Score a, Score b) {return a.score > b.score; });
    file.close();
}

void writeScore() {
    Score score{ gI.playerName, gI.points };
    std::fstream file;
    file.open(scoresFileName, std::ios::app);
    std::string newScore = score.playerName + "," + std::to_string(score.score) + "\n";
    file << newScore;
    file.close();
}