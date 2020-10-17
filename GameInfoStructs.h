#pragma once
#include <vector>
#include <string>

struct Score {
    string playerName;
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
    double time;
    string playerName;
    GameStates gameState;
    vector<Score> highScoresList;
};