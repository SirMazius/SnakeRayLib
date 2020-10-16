#include "raylib.h"
#include "Snake.h"
#include "Board.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "raymath.h"
#include <fstream>
#include <algorithm>

using namespace std;
/*

*/

struct Score {
    std::string playerName;
    int score;
};

enum GameStates
{
    logoScreen = 0,
    titleScreen = 1,
    gameScreen = 2,
    gamePause = 3,
    gameOver = 4,
    gameReset = 5,
    levelSelectionScreen = 6
};

struct GameInfo {
    int points;
    GameStates gameState = GameStates::gameOver;
    std::vector<Score> highScoresList;
} gI;



void keyInput(Snake & s);
void checkSnakeMovement(Snake & s); // Comprueba al final de cada movimiento si hay comida o impacto
void drawIntroLogo(Texture2D * rayLibLogoTexture, double & windowTimeOffset);
void drawStartMenu(const Font * customFont, double windowTimeOffset);
void updateGame(Snake & s);
void drawPauseMenu();
void drawEndGameMenu(const Font* customFont);
void relocate(Snake& s);
void initGameVariables(Texture2D * rlLogo, Font * customFont, Snake & s);
void loadHighScores();
void writeScore();
void drawPoints();
void drawLevelSelectionMenu(Snake & s);
const int screenWidth = 800;
const int screenHeight = 900;
const int NAMEMAXCHAR = 3;
std::string playerName;

const std::string scoresFileName = "highScores.dat";

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
    
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        static double windowTimeOffset = GetTime(); // TODO: Esto se puede sacar de aqui

        BeginDrawing();


        ClearBackground(RAYWHITE);
         
        switch (gI.gameState) {
        case 0: // Anim intro
            drawIntroLogo(&rayLibLogoTexture, windowTimeOffset);
            break;

        case 1: // Start Menu
            drawStartMenu(&customFont, windowTimeOffset);
            keyInput(s);
            break;
        case 2: // Game
            keyInput(s);
            updateGame(s);
            drawPoints();
            s.Draw();
            Board::Draw();
            break;
        case 3: // GamePaused
            keyInput(s);
            drawPoints();
            s.Draw();
            Board::Draw();
            drawPauseMenu();
            break;
        case 4: // End Game
            keyInput(s);
            drawEndGameMenu(&customFont);
            break;
        case 5: // Reset
            initGameVariables(&rayLibLogoTexture, &customFont, s);
            gI.gameState = GameStates::titleScreen;
            break;
        case 6: // level selection
            drawLevelSelectionMenu(s);
            break;
        }
       
        

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(rayLibLogoTexture);
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void drawPoints() {
    string pointsText = "Points : " + to_string(gI.points);
    DrawText(pointsText.c_str() , screenWidth / 2.8, screenHeight - 70, 40, BLACK);
}

void keyInput(Snake & s) { // TODO: Mover el init board aqui
    auto & gameState = gI.gameState;
    if (gameState == (int)GameStates::titleScreen) {
        if (IsKeyPressed(KEY_SPACE)) {
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
            gameState = GameStates::gameScreen;
        }
    }
    else if (gameState == (int)GameStates::gameOver) {
        int key = GetKeyPressed();
        std::cout << key << std::endl;
        if (IsKeyPressed(KEY_BACKSPACE)) {
            playerName.pop_back();
        }
        else if (playerName.length() < NAMEMAXCHAR && (key >= 32) && (key <= 125) && (key != KEY_SPACE)) {
            playerName += char(key);
        }
        else if (IsKeyPressed(KEY_ENTER) && playerName.length() == NAMEMAXCHAR) {
            writeScore();
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


void drawIntroLogo(Texture2D * t, double & windowTimeOffset) {
    auto & gameState = gI.gameState;
    double time = GetTime() - windowTimeOffset;
    std::cout << "time:"+ std::to_string(time) << std::endl;
    float opacity;
    if (time <= 1) {
        opacity = sin(time * 1.5707f);
        std::cout << opacity << std::endl;
    }
    else if (time > 1 && time <= 3) {
        opacity = 1;
    }
    else if (time > 3 && time <= 5) {
        opacity = cos((time - 3) * 1.5707f);
    }
    else {
        gameState = GameStates::titleScreen;
        windowTimeOffset = GetTime();
        return;
    }
    DrawTexture(*t, screenWidth / 2 - t->width / 2, screenHeight / 2 - t->height / 2, Fade(WHITE, opacity));
}

void drawLevelSelectionMenu(Snake & s) {
    DrawText("Select level", screenWidth/3.0f, 100, 40, BLACK);
    vector<Rectangle> buttons;

    for (int i = 0; i < Board::levels.size(); i++) {
        //buttons.push_back({ screenWidth / 2 - 50, 150 + i * 10, 78, 50 });
        Rectangle button { screenWidth / 2 - 50, 150 + i * 60, 78, 50 };
        DrawText(to_string(i).c_str(), button.x + 33, button.y + 10, 20, BLACK);
        if ((CheckCollisionPointRec(GetMousePosition(), button))) {
            DrawRectangleLines(button.x, button.y, button.width, button.height, RED);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Board::CreateBoard(i);
                gI.gameState = GameStates::gameScreen;
                Board::PlaceFood();
            }
        } 
        else {
            DrawRectangleLines(button.x, button.y, button.width, button.height, BLACK);
        } 
        
    }
}

void drawStartMenu(const Font * customFont, double windowTimeOffset) {
    // static float time = 0;
    double time = GetTime() - windowTimeOffset;
    const Vector2 initPos { screenWidth / 6, screenHeight / 4 - 600 };
    const Vector2 finalPos { screenWidth / 6, screenHeight / 4 };
    static Vector2 currentPos;
    if (time < 2.0f) { // Hacemos caer el texto desde arriba
        currentPos.x = initPos.x;
        currentPos.y = Lerp(initPos.y, finalPos.y, time / 2.0f);
        DrawTextEx(*customFont,"SNAKE",currentPos,customFont->baseSize * 20,8,BLACK);
    }
    else { // Start a parpadeando
        currentPos = finalPos;
        DrawTextEx(*customFont, "SNAKE!", currentPos, customFont->baseSize * 20, 8, BLACK);

        if (sin(time * 5) > 0) {
            DrawText("PRESS START to PLAY", GetScreenWidth() / 3, GetScreenHeight() / 1.5f, 20, LIGHTGRAY);
        }
    }
}

void drawPauseMenu() {
    double time = GetTime();
    if (sin(time * 5) > 0) {
        DrawText("PRESS START to PLAY", GetScreenWidth() / 3, GetScreenHeight() / 1.5f, 20, LIGHTGRAY);
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
    loadHighScores();
    s = Snake(9, 9);
    playerName = "";
}

void drawEndGameMenu(const Font* customFont) {
    Vector2 finalPos{ screenWidth / 6, screenHeight / 4 };
    const Rectangle textBox = { screenWidth / 2-50, 380, 78, 50 };
    std::vector<Score> & scoreList = gI.highScoresList;
    std::string scoreText = "Score : " + std::to_string(gI.points);
    DrawText(scoreText.c_str(), finalPos.x, finalPos.y + 100, 40, BLACK);
    for (int i = 0; i < 10 && i < scoreList.size(); i++) {
        // TODO: Escribimos aqui las puntuaciones
        DrawText((scoreList.at(i).playerName + " " + std::to_string(scoreList.at(i).score)).c_str(), textBox.x, textBox.y + 60 + i * 20, 20, BLACK);
    }
    finalPos.y -= 100;
    DrawTextEx(*customFont, "GAME OVER", finalPos, customFont->baseSize * 10, 8, BLACK);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);
    DrawText(playerName.c_str(), textBox.x + 5, textBox.y + 8, 40, BLACK);

    if (sin(GetTime() * 2) > 0) {
        DrawText("Fill your name and press ENTER", GetScreenWidth() / 3.5f, GetScreenHeight() / 1.1f, 20, LIGHTGRAY);
    }
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
    Score score{ playerName, gI.points };
    std::fstream file;
    file.open(scoresFileName, std::ios::app);
    std::string newScore = score.playerName + "," + std::to_string(score.score) + "\n";
    file << newScore;
    file.close();
}