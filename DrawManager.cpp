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
    // Dibujamos o no el titulo solo en los valores positivos del seno.
	if (sin(animationTime * 5) > 0) { 
		DrawText("Press ENTER to PLAY", GetScreenWidth() / 3, GetScreenHeight() / 1.5f, 20, LIGHTGRAY);
	}
}

void DrawManager::DrawStartMenu() {
    const Vector2 initPos{ screenWidth / 6, screenHeight / 4 - 600 };
    const Vector2 finalPos{ screenWidth / 6, screenHeight / 4 };
    static Vector2 currentPos;

    // Durante los dos primeros segundos haremos caer el texto del cielo.
    if (animationTime < 2.0f) { 
        currentPos.x = initPos.x;
        // Interpolamos la altura entre la posicion inicial y final del texto.
        currentPos.y = Lerp(initPos.y, finalPos.y, animationTime / 2.0f);
        DrawTextEx(*customFont, "SNAKE", currentPos, customFont->baseSize * 20, 8, BLACK);
    }
    else { // Cuando el texto haya terminado de caer, hacemos aparecer el texto que parpadea.
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

/*
    Hacemos aparecer la animacion con un efecteo fade-in fade-out.
*/
void DrawManager::DrawIntroLogo() {
    float opacity;
    // Durante el primer segundo aumentandomos la opacidad progresivamente.
    if (animationTime <= 1) {
        opacity = sin(animationTime * 1.5707f);
    }
    // Cuando el logo es completamente visible lo mantenemos durante 2 segundos.
    else if (animationTime > 1 && animationTime <= 3) {
        opacity = 1;
    }
    // Una vez han pasado 3 segundos, disolvemos el texto.
    else if (animationTime > 3 && animationTime <= 5) {
        opacity = cos((animationTime - 3) * 1.5707f);
    }
    // Terminada la animacion pasamos a la pantalla donde se muestra la animacion del titulo.
    else {
        gameInf->gameState = GameStates::titleScreen;
        animationTime = 0;
        return;
    }
    // Dibujamos el logo el cual se mostrara o no en funcion de la opacidad.
    DrawTexture(*logoTexture, screenWidth / 2 - logoTexture->width / 2, screenHeight / 2 - logoTexture->height / 2, Fade(WHITE, opacity));
}

void DrawManager::DrawLevelSelectionMenu() {
    DrawText("Select level", screenWidth / 3.0f, 100, 40, BLACK);

    // Iteramos sobre todos los niveles que se han cargado.
    for (int i = 0; i < Board::levels.size(); i++) {
        // Dibujamos cada boton con un cierto desfase.
        Rectangle button{ screenWidth / 2 - 50, 150 + i * 60, 78, 40 };
        DrawText(to_string(i).c_str(), button.x + 33, button.y + 10, 20, BLACK);
        //Comprobamos si el raton entra en el boton, en el caso de que  entre cambiamos el color.
        if ((CheckCollisionPointRec(GetMousePosition(), button))) {
            DrawRectangleLines(button.x, button.y, button.width, button.height, RED);
            // Si estando dentro del boton pulsamos, cargamos el nivel y empezamos el juego.
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Board::CreateBoard(i);
                gameInf->gameState = GameStates::gameScreen;
                Board::PlaceFood();
            }
        }
        // Si el raton no interactua con el boton lo dibujamos en su color de base.
        else {
            DrawRectangleLines(button.x, button.y, button.width, button.height, BLACK);
        }

    }
}

void DrawManager::DrawEndGameMenu() {
    Vector2 finalPos{ screenWidth / 6, screenHeight / 4 };
    // Rectangulo donde el jugador escribira su nombre.
    const Rectangle textBox = { screenWidth / 2 - 50, 380, 78, 50 };

    vector<Score>& scoreList = gameInf->highScoresList;
    string scoreText = "Score : " + to_string(gameInf->points);
    // Pintamos la puntuacion que ha logrado el jugador durante la partida.
    DrawText(scoreText.c_str(), finalPos.x, finalPos.y + 100, 40, BLACK);
    // Iteramos sobre las 10 mejores puntuaciones.
    for (int i = 0; i < 10 && i < scoreList.size(); i++) {
        DrawText((scoreList.at(i).playerName + " " + to_string(scoreList.at(i).score)).c_str(), textBox.x, textBox.y + 60 + i * 20, 20, BLACK);
    }
    finalPos.y -= 100;
    // Mostramos el texto de GAME OVER.
    DrawTextEx(*customFont, "GAME OVER", finalPos, customFont->baseSize * 10, 8, RED);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, BLACK);
    // Nos aseguramos que los caracteres que introduce el jugador son en minusculas.
    transform(gameInf->playerName.begin(), gameInf->playerName.end(), gameInf->playerName.begin(), ::tolower);
    DrawText(gameInf->playerName.c_str(), textBox.x + 5, textBox.y + 8, 40, BLACK);

    if (sin(GetTime() * 2) > 0) {
        DrawText("Fill your name and press ENTER", GetScreenWidth() / 3.5f, GetScreenHeight() / 1.1f, 20, LIGHTGRAY);
    }
}