#include "Snake.h"

Snake::Snake(int x, int y) {
    bodyList.clear();
    foodList.clear();
    currentDirection = Left;
    lastDirection = Left;
    bodyList.push_back(Vector2{ (float)x, (float)y });
    bodyList.push_back(Vector2{ (float)x+1, (float)y });
    bodyList.push_back(Vector2{ (float)x+2, (float)y });
}

Snake::Snake() {
    int x, y;
    x = y = 9;
    bodyList.clear();
    foodList.clear();
    currentDirection = Left;
    lastDirection = Left;
    bodyList.push_back(Vector2{ (float)x, (float)y });
    bodyList.push_back(Vector2{ (float)x + 1, (float)y });
    bodyList.push_back(Vector2{ (float)x + 2, (float)y });
}

/*
    Movemos el cuerpo de la serpiente de atras adelante hasta 
    el ultimo segmento antes de la cabeza.
*/
void Snake::MoveBody() {
    for (auto it = bodyList.rbegin(); it != bodyList.rend(); it++) { 
        auto auxIt = it;
        auxIt++;
        if (auxIt != bodyList.rend()) {
            it->x = auxIt->x;
            it->y = auxIt->y;
        }
    }
}

/*
    En funcion de la tecla que hayamos pulsado movemos la cabeza.
*/
void Snake::MoveHead() {
    auto headIt = bodyList.begin();
    switch (currentDirection) {
    case Up:
        headIt->y--;
        break;
    case Down:
        headIt->y++;
        break;
    case Left:
        headIt->x--;
        break;
    case Right:
        headIt->x++;
        break;
    }

}

/*
    Mueve la serpiente entera y hace crecer la serpiente en
    caso de que sea necesario.
*/
void Snake::Move() {
    Vector2 posTail = bodyList.back();

    MoveBody();
    MoveHead();
    // Comprobamos si el final de la serpiente coincide con comida, si es asi hacemos crecer la serpiente.
    if (!foodList.empty() && posTail.x == foodList.front().x && posTail.y == foodList.front().y) {
        bodyList.push_back(foodList.front());
        foodList.pop_front();
    }
}
/*
    Comprobamos la colision de la cabeza de la serpiente con su cuerpo.
*/
bool Snake::CheckCollision() {
    auto headIt = bodyList.begin();
    for (auto it = ++bodyList.begin(); it != bodyList.end(); it++) {
        if (headIt->x == it->x && headIt->y == it->y) {
            return true;
        }
    }
    return false;
}

void Snake::Eat() {
    foodList.push_back(*bodyList.begin());
}

void Snake::GoUp() {
    if (lastDirection != Down) {
        currentDirection = Up;
    }
}

void Snake::GoDown() {
    if (lastDirection != Up) {
        currentDirection = Down;
    }
}

void Snake::GoRight() {
    if (lastDirection != Left) {
        currentDirection = Right;
    }
}

void Snake::GoLeft() {
    if (lastDirection != Right) {
        currentDirection = Left;
    }
}
/*
    lastDirection nos permite asegurarnos de que la serpiente no puede hacer
    giros de 180.
*/
void Snake::UpdateLastDirection() {
    lastDirection = currentDirection;
}

/*
    Dibujamos la serpiente en rojo la cabeza y en verde el cuerpo.
*/
void Snake::Draw() { 

    Vector2 upperLeftVec;
    Vector2 lowerRightVec;
    // Coordenadas de la cabeza.
    upperLeftVec.x = bodyList.begin()->x * 50 + 10;
    upperLeftVec.y = bodyList.begin()->y * 50 + 10;
    lowerRightVec.x = 30;
    lowerRightVec.y = 30;
    // Dibujamos la cabeza.
    DrawRectangleV(upperLeftVec, lowerRightVec, RED);

    // Recorremos la lista del cuerpo.
    for (auto snakeChunkIt = ++bodyList.begin(); snakeChunkIt != bodyList.end(); snakeChunkIt++) {
        upperLeftVec.x = snakeChunkIt->x * 50 + 10;
        upperLeftVec.y = snakeChunkIt->y * 50 + 10;
        lowerRightVec.x = 30;
        lowerRightVec.y = 30;
        // Dibujamos cada pedazo del cuerpo.
        DrawRectangleV(upperLeftVec, lowerRightVec, GREEN);
    }
    
}
