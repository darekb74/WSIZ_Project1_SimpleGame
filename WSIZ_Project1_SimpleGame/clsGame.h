#ifndef CLSGAME_H
#define CLSGAME_H
#pragma once

#include "clsMatrix.h"

using namespace System;

class clsGame
{
public:
	UInt16 level, balls;
	Boolean start;
public:
	clsGame();
	void gameStart(clsMatrix *, UInt16);
};
#endif