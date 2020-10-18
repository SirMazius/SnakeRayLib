#pragma once
#include <vector>
#include <string>

/*
    Estructuras para almacenar informacion del juego.
*/


/*
    Contenedor para el cargado de las puntuaciones.
*/
struct Score {
    string playerName;
    int score;
};


/*
    Enum para facilitar la lectura del estado en el que
    nos encontramos.
*/
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
/*
    Variables de la partida.
*/
struct GameInfo {
    int points;
    string playerName;
    GameStates gameState;
    vector<Score> highScoresList;
};