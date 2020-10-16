
#pragma once

#include <list>
#include <raylib.h>

class Snake {

    /*
        Solo refrescamos la pantalla cuando se mueve ?? look mas clasic
    */

    void MoveBody();
    void MoveHead(); // Se encarga de mover la cabeza en funcion de la direccion
    // void Grow
    enum Direction { Up = 8, Down = 2, Left = 4, Right = 6 };

public:

    Snake(int x, int y); // Crea a la serpiente en algun lugar del mapa y con una direccion
    Snake();
    void Draw();
    void Move(); // Cada vez que te vas a mover compruebas si el final coincide con comida, si coincide mueves todo y creas una al final si al acabar de mover coincide la cabeza con alguna parte de la serpiente u obstaculo pues pierdes
    void GoUp();
    void GoDown();
    void GoRight();
    void GoLeft();
    void UpdateLastDirection();
    void Eat(); // Guardamos la posicion del objeto que nos hemos comido, cuando el final de la serpiente pase por ahi hay que crecer
    bool CheckCollision(); // Comprueba colision contra todo
    std::list<Vector2> bodyList;
    std::list<Vector2> foodList;
    Direction currentDirection;
    Direction lastDirection;

};
