#include "DrawManager.h"
using namespace std;

Snake* DrawManager::snake;
Font* DrawManager::customFont;
GameInfo* DrawManager::gameInf;
Texture2D* DrawManager::logoTexture;
double DrawManager::animationTime;
float DrawManager::screenWidth;
float DrawManager::screenHeight;

void DrawManager::InitDrawManager(Snake* s, Font* cF, GameInfo* gI, Texture2D* t) {
	snake = s;
	customFont = cF;
	gameInf = gI;
	logoTexture = t;
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    animationTime = 0;
}

void DrawManager::DrawPauseMenu() {
	double time = GetTime();
	if (sin(animationTime * 5) > 0) {
		DrawText("PRESS START to PLAY", GetScreenWidth() / 3, GetScreenHeight() / 1.5f, 20, LIGHTGRAY);
	}
}

void DrawManager::DrawStartMenu() {
    const Vector2 initPos{ screenWidth / 6, screenHeight / 4 - 600 };
    const Vector2 finalPos{ screenWidth / 6, screenHeight / 4 };
    static Vector2 currentPos;
    if (animationTime < 2.0f) { // Hacemos caer el texto desde arriba
        currentPos.x = initPos.x;
        currentPos.y = Lerp(initPos.y, finalPos.y, animationTime / 2.0f);
        DrawTextEx(*customFont, "SNAKE", currentPos, customFont->baseSize * 20, 8, BLACK);
    }
    else { // Start a parpadeando
        currentPos = finalPos;
        DrawTextEx(*customFont, "SNAKE!", currentPos, customFont->baseSize * 20, 8, BLACK);

        if (sin(animationTime * 5) > 0) {
            DrawText("PRESS START to PLAY", GetScreenWidth() / 3, GetScreenHeight() / 1.5f, 20, LIGHTGRAY);
        }
    }
}

void DrawManager::DrawPoints() {
    string pointsText = "Points : " + to_string(gameInf->points);
    DrawText(pointsText.c_str(), screenWidth / 2.8, screenHeight - 70, 40, BLACK);
}

void DrawManager::DrawIntroLogo() {
    // std::cout << "time:" + std::to_string(time) << std::endl;
    float opacity;
    if (animationTime <= 1) {
        opacity = sin(animationTime * 1.5707f);
        /*std::cout << opacity << std::endl;*/
    }
    else if (animationTime > 1 && animationTime <= 3) {
        opacity = 1;
    }
    else if (animationTime > 3 && animationTime <= 5) {
        opacity = cos((animationTime - 3) * 1.5707f);
    }
    else {
        gameInf->gameState = GameStates::titleScreen;
        animationTime = 0;
        return;
    }
    DrawTexture(*logoTexture, screenWidth / 2 - logoTexture->width / 2, screenHeight / 2 - logoTexture->height / 2, Fade(WHITE, opacity));
}

void DrawManager::DrawLevelSelectionMenu() {
    DrawText("Select level", screenWidth / 3.0f, 100, 40, BLACK);
    vector<Rectangle> buttons;

    for (int i = 0; i < Board::levels.size(); i++) {
        //buttons.push_back({ screenWidth / 2 - 50, 150 + i * 10, 78, 50 });
        Rectangle button{ screenWidth / 2 - 50, 150 + i * 60, 78, 40 };
        DrawText(to_string(i).c_str(), button.x + 33, button.y + 10, 20, BLACK);
        if ((CheckCollisionPointRec(GetMousePosition(), button))) {
            DrawRectangleLines(button.x, button.y, button.width, button.height, RED);
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Board::CreateBoard(i);
                gameInf->gameState = GameStates::gameScreen;
                Board::PlaceFood();
            }
        }
        else {
            DrawRectangleLines(button.x, button.y, button.width, button.height, BLACK);
        }

    }
}

void DrawManager::DrawEndGameMenu() {
    Vector2 finalPos{ screenWidth / 6, screenHeight / 4 };
    const Rectangle textBox = { screenWidth / 2 - 50, 380, 78, 50 };
    std::vector<Score>& scoreList = gameInf->highScoresList;
    std::string scoreText = "Score : " + std::to_string(gameInf->points);
    DrawText(scoreText.c_str(), finalPos.x, finalPos.y + 100, 40, BLACK);
    for (int i = 0; i < 10 && i < scoreList.size(); i++) {
        // TODO: Escribimos aqui las puntuaciones
        DrawText((scoreList.at(i).playerName + " " + std::to_string(scoreList.at(i).score)).c_str(), textBox.x, textBox.y + 60 + i * 20, 20, BLACK);
    }
    finalPos.y -= 100;
    DrawTextEx(*customFont, "GAME OVER", finalPos, customFont->baseSize * 10, 8, BLACK);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
    std::transform(gameInf->playerName.begin(), gameInf->playerName.end(), gameInf->playerName.begin(), ::tolower);
    DrawText(gameInf->playerName.c_str(), textBox.x + 5, textBox.y + 8, 40, BLACK);

    if (sin(GetTime() * 2) > 0) {
        DrawText("Fill your name and press ENTER", GetScreenWidth() / 3.5f, GetScreenHeight() / 1.1f, 20, LIGHTGRAY);
    }
}