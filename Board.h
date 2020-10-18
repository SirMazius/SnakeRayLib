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

/*
	Clase encargada de la gestion del tablero,
	de cargar los niveles, colocar los snacks y
	dibujarlo.
*/
class Board { 
		static int screenWidth, screenHeight;
public :
	enum Square { Empty = 0, Food = 1, Obstacle = 2, SnakeBody = 3 };
	static const int TAM = 16;
	static Square board[TAM][TAM];
	static vector<string> levels;
	static Snake * snake;
	static Vector2 snak1;
	static Vector2 snak2;

	static void DrawBoard();
	static void DrawCells();
	static void Draw();
	// Crea el tablero, cargando el nivel en funcion del modo seleccionado.
	static void CreateBoard(int mode);
	// Inicializa las varibales estaticas del tablero.
	static void InitBoard(Snake * s);
	static void PlaceFood();
	static void LoadBoards();
	};
