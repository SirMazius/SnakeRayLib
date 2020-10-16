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

void Snake::MoveBody() {
    for (auto it = bodyList.rbegin(); it != bodyList.rend(); it++) { // Actualizamos desde detras de la cabeza hasta el final
        auto auxIt = it;
        auxIt++;
        if (auxIt != bodyList.rend()) {
            it->x = auxIt->x;
            it->y = auxIt->y;
        }
    }
}

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

void Snake::Move() {
    Vector2 posTail = bodyList.back();

    MoveBody();
    MoveHead();

    if (!foodList.empty() && posTail.x == foodList.front().x && posTail.y == foodList.front().y) { // Si el final coincide con la comida hay que a�adir
        bodyList.push_back(foodList.front());
        foodList.pop_front();
    }
}

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

void Snake::UpdateLastDirection() {
    lastDirection = currentDirection;
}

void Snake::Draw() { // Cuadrados negros para el cuerpo y rojos para la cabeza

    Vector2 upperLeftVec;
    Vector2 lowerRightVec;

    upperLeftVec.x = bodyList.begin()->x * 50 + 10;
    upperLeftVec.y = bodyList.begin()->y * 50 + 10;
    lowerRightVec.x = /*bodyList.begin()->x * 50 + */30;
    lowerRightVec.y = /*bodyList.begin()->y * 50 +*/ 30;

    DrawRectangleV(upperLeftVec, lowerRightVec, RED);

    for (auto snakeChunkIt = ++bodyList.begin(); snakeChunkIt != bodyList.end(); snakeChunkIt++) {
        upperLeftVec.x = snakeChunkIt->x * 50 + 10;
        upperLeftVec.y = snakeChunkIt->y * 50 + 10;
        lowerRightVec.x = /*bodyList.begin()->x * 50 + */30;
        lowerRightVec.y = /*bodyList.begin()->y * 50 +*/ 30;
        DrawRectangleV(upperLeftVec, lowerRightVec, GREEN);
    }
    //DrawRectangleV(Vector2 position, Vector2 size, Color color);
}
