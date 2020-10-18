#pragma once

#include "Snake.h"
#include "Board.h"
#include "GameInfoStructs.h"
#include "raymath.h"
#include <string>
#include <iostream>

/*
	Clase encargada del dibujado de las diferentes pantallas del juego.
*/
class DrawManager
{
	static Snake* snake;
	static Font* customFont;
	static GameInfo* gameInf;
	static Texture2D* logoTexture;
	static float screenWidth, screenHeight;
	
public:
	static double animationTime;
	static void InitDrawManager(Snake* s, Font* cF, GameInfo* gI, Texture2D* t);
	static void DrawPauseMenu(); 
	static void DrawStartMenu(); 
	static void DrawPoints(); 
	static void DrawIntroLogo(); 
	static void DrawLevelSelectionMenu(); 
	static void DrawEndGameMenu(); 
};

