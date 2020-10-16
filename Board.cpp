#include "Board.h"


Board::Square Board::board[Board::TAM][Board::TAM];
vector<string> Board::levels;
int Board::screenWidth;
int Board::screenHeight;
Snake* Board::snake;
Vector2 Board::snak1;
Vector2 Board::snak2;


void Board::CreateBoard(int mode = 0) {
    srand((unsigned)time(0)); // Inicializamos la seed
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    for (int i = 0; i < TAM; i++) { // Limpiamos el tablero asignado Empty a todo
        for (int j = 0; j < TAM; j++) {
            
            if (mode == 0) {
                board[i][j] = Empty;
            }
            else 
            {
                string level = levels.at(mode-1);
                char cellInfo = level.at(i + j * TAM);
                board[i][j] = cellInfo == '0' ? Empty : Obstacle;
            }
            
        }
    }
}

void Board::InitBoard(Snake * s) {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    snake = s;
    levels.clear();
}

void Board::LoadBoards() {
    const regex nameRegex("level\\d.csv");
    fstream f;
    auto path = filesystem::current_path();
    for (const auto& entry : filesystem::directory_iterator(path)) {
        if (regex_match(entry.path().filename().string(), regex(nameRegex))) { // Regex para los niveles o ponerlos a pelo
            f.open(entry.path().filename(), ios::in);
            string line = "";
            string level = "";
            while (getline(f, line)) {
                if (line != "\n") { // !f.eof()
                    for (int i = 0; i < line.length(); i++) {
                        if (line.at(i) == '0' || line.at(i) == '1') {
                            level += line.at(i);
                        }
                     }
                }
            }
            levels.push_back(level);
            f.close();
        }
    }
}

void Board::PlaceFood() {
    int randX;
    int randY;
    for (int i = 0; i < 2; i++) {
        bool foodPlaced = false;
        while (!foodPlaced)
        {
            randX = rand() % TAM;
            randY = rand() % TAM;

            foodPlaced = true;
            for (const Vector2& snakeChunk : snake->bodyList)
            {
                if (board[randX][randY] == Food  || board[randX][randY] == Obstacle || ((int)snakeChunk.x == randX && (int)snakeChunk.y == randY)) {
                    foodPlaced = false;
                    break;
                }
            }
        }
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
