#ifndef CLSMATRIX_H
#define CLSMATRIX_H
#pragma once

#include "WSIZ_Project1_SimpleGame.h"
#include "clsBall.h"
#include <string>

using namespace System;

class clsMatrix
{
public:
	//array<Int16, 2>^ tabBricks;
	struct Level
	{
		Int16 nextLevel;
		std::string lvlDesign;
	};
	Level dbLevels[10]; // 10 poziomów
	Int16 tabBricks[mLINES+1][20];
	Boolean endLevel;

	clsMatrix(void);
	void showBricks(Int16);
	void printBrick(Int16 x, Int16 y, Int16 type, UInt16 backColor = 0);
	void checkMatrix (clsBall *);
	void flashMatrix(UInt16 color);
	Boolean loadLevels();
	UInt16 countBricks();
private:
	Int16 tabBValue[10];
	Int16 tabBTrans[10];
	void playSound(UInt16 brickType);
	Boolean changeWer(clsBall *tmpBall, Int16 mX, Int16 mY, Int16 mcX, Int16 mcY, Boolean justCheck = false);
	void brickExplode(UInt16 mX, UInt16 mY);
};
#endif
