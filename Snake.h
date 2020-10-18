
#pragma once

#include <list>
#include <raylib.h>

/*
    Gestiona el movimiento de la serpiente asi como su interaccion
    consigo misma y el manejo de la comida.
*/

class Snake {
    void MoveBody();
    void MoveHead(); 
    enum Direction { Up = 8, Down = 2, Left = 4, Right = 6 };

public:
    // Crea a la serpiente en algun lugar del mapa.
    Snake(int x, int y); 
    Snake();
    void Draw();
    void Move(); 
    void GoUp();
    void GoDown();
    void GoRight();
    void GoLeft();
    void UpdateLastDirection();
    // Guardamos la posicion del objeto que nos hemos comido, cuando el final de la serpiente pase por ahi hay que crecer.
    void Eat(); 
    // Comprueba colision con las partes del cuerpo.
    bool CheckCollision(); 
    std::list<Vector2> bodyList;
    std::list<Vector2> foodList;
    Direction currentDirection;
    Direction lastDirection;

};
