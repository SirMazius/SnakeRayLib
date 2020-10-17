#pragma once

#include "Snake.h"
#include "Board.h"
#include "GameInfoStructs.h"
#include "raymath.h"
#include <string>
#include <iostream>
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
	static void DrawPauseMenu(); // ok
	static void DrawStartMenu(); // ok
	static void DrawPoints(); // ok
	static void DrawIntroLogo(); // ok
	static void DrawLevelSelectionMenu(); // ok
	static void DrawEndGameMenu();
};

