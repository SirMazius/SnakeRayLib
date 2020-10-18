#include "Board.h"


Board::Square Board::board[Board::TAM][Board::TAM];
vector<string> Board::levels;
int Board::screenWidth;
int Board::screenHeight;
Snake* Board::snake;
Vector2 Board::snak1;
Vector2 Board::snak2;

/*
    En funcion del modo seleccionado creamos el tablero
    inicializando las casillas a EMPTY y poniendo los obstaculos.
*/
void Board::CreateBoard(int mode = 0) {
    srand((unsigned)time(0)); // Inicializamos la seed
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    for (int i = 0; i < TAM; i++) { 
        for (int j = 0; j < TAM; j++) {
            // Limpiamos el tablero asignado Empty a todo.
            if (mode == 0) {
                board[i][j] = Empty;
            }
            else 
            {
                // Ponemos los obstaculos en funcion del nivel.
                string level = levels.at(mode-1);
                char cellInfo = level.at(i + j * TAM);
                board[i][j] = cellInfo == '0' ? Empty : Obstacle;
            }
            
        }
    }
}

/*
Inicializa las varibales estaticas del tablero.
*/
void Board::InitBoard(Snake * s) {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    snake = s;
    levels.clear();
}

/*
    Cargamos la lista de niveles, los cuales han de tener el siguiente formato
    level<numero>.csv y encontrarse compuestos de 0 y 1 en funcion de el estado
    de la celda 0 para vacias y 1 para obstaculos.
*/
void Board::LoadBoards() {
    // Expresion regular para verficar el nombre del fichero.
    const regex nameRegex("level\\d.csv");
    fstream f;
    auto path = filesystem::current_path() /= "Resources";
    
    // Recorremos los ficheros de la carpeta Resources.
    for (const auto& entry : filesystem::directory_iterator(path)) {
        // En caso de que el fichero cumpla con la expresion regular lo procesamos.
        if (regex_match(entry.path().filename().string(), regex(nameRegex))) {
            f.open(entry.path(), ios::in);
            string line = "";
            string level = "";
            // Recorremos linea a linea el fichero y almacenamos los en un string
            while (getline(f, line)) {
                if (line != "\n") {
                    for (int i = 0; i < line.length(); i++) {
                        if (line.at(i) == '0' || line.at(i) == '1') {
                            level += line.at(i);
                        }
                     }
                }
            }
            // Anyadimos el string con la informacion del nivel a la lista de niveles.
            levels.push_back(level);
            f.close();
        }
    }
    
}
/*
    Crea la comida de la serpiente teniendo en cuenta que esta no cae en
    una casilla en la que se encuentre un obstaculo o la serpiente.
*/
void Board::PlaceFood() {
    int randX;
    int randY;
    // Colocaremos dos unidades de comida.
    for (int i = 0; i < 2; i++) {
        bool foodPlaced = false;
        while (!foodPlaced)
        {
            randX = rand() % TAM;
            randY = rand() % TAM;

            foodPlaced = true;
            // Recorremos la serpiente comprobando que no colocamos comida encima de ella.
            for (const Vector2& snakeChunk : snake->bodyList)
            {
                if ( (int)snakeChunk.x == randX && (int)snakeChunk.y == randY) {
                    foodPlaced = false;
                    break;
                } 
            }

            // Comprobamos que no colocamos comida sobre comida o sobre un obstaculo.
            if (board[randX][randY] == Food || board[randX][randY] == Obstacle) {
                foodPlaced = false;
            }
        }
        // Colocamos la comida y la asignamos para saber donde esta.
        board[randX][randY] = Food;
        if (i == 0) {
            snak1.x = randX;
            snak1.y = randY;
        }
        else {
            snak2.x = randX;
            snak2.y = randY;
        }
        
    }
    
}

/*
    Dibujamos las lineas que componen el tablero.
*/
void Board::DrawBoard() {
    int screenHeight = 800;
    for (int i = 0; i < TAM; i++) {
        
        Vector2 verticalVecA { i * (screenWidth / TAM), 0 };
        Vector2 verticalVecB { i * (screenWidth / TAM), screenHeight};
        Vector2 horizontalVecA { 0, i * screenHeight / TAM};
        Vector2 horizontalVecB { screenWidth, i * screenHeight / TAM };
        DrawLineV(verticalVecA, verticalVecB, Color{0,0,0,255});
        DrawLineV(horizontalVecA, horizontalVecB, Color{ 0,0,0,255 });
    }

    Vector2 horizontalVecA{ 0, screenHeight };
    Vector2 horizontalVecB{ screenWidth, screenHeight};
    DrawLineV(horizontalVecA, horizontalVecB, Color{ 0,0,0,255 });
}

/*
    Recorremos cada celda dibujando la comida o los obstaculos.
*/
void Board::DrawCells() {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            Vector2 upperLeftVec;
            Vector2 lowerRightVec;

            upperLeftVec.x = i * 50 + 10;
            upperLeftVec.y = j * 50 + 10;
            lowerRightVec.x = 30;
            lowerRightVec.y = 30;

            switch (board[i][j]) {
                case Food:
                DrawRectangleV(upperLeftVec, lowerRightVec, BLUE);
                break;
                case Obstacle:
                DrawRectangleV(upperLeftVec, lowerRightVec, ORANGE);
                break;
            }
        }
    }
}

void Board::Draw() {
    DrawBoard();
    DrawCells();
}
