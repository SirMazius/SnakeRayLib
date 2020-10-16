#pragma once
#include "Snake.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <regex>
#include <fstream>

using namespace std;
class Board {

	    static const int TAM = 16;

		static int screenWidth, screenHeight;
		

	     // Limpiamos todo el tablero y en funcion del modo de juego cargamos unos obstaculos u otros

	    // void PlaceFood(const Snake s); // Necesitamos saber donde est� la serpiente para no calzarle comida justo encima
public :
	enum Square { Empty = 0, Food = 1, Obstacle = 2, SnakeBody = 3 };
	static Square board[TAM][TAM];
	static vector<string> levels;
	static Snake * snake;
	static Vector2 snak1;
	static Vector2 snak2;
	static void DrawBoard();
	static void DrawCells();
	static void Draw();
	static void CreateBoard(int mode);
	static void InitBoard(Snake * s);
	static void PlaceFood();
	static void LoadBoards();
	};
