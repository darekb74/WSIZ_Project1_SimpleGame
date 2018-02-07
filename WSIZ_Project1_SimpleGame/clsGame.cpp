#include "stdafx.h"
#include "windows.h"
#include "clsGame.h"

extern UInt32 wynik;

clsGame::clsGame()
{
	level = 0;
	balls = 3;
	start = false;
	wynik = 0;
}

void clsGame::gameStart(clsMatrix *tmpMatrix, UInt16 level)
{
	balls = 3;
	wynik = 0;
	start = true;
	tmpMatrix->showBricks(level);
}
