#include "stdafx.h"
#include "windows.h"
#include "clsPlatform.h"
#include "WSIZ_Project1_SimpleGame.h"

clsPlatform::clsPlatform()
{
	posX = 56;
	speed = 1;
}

void clsPlatform::move()
{
	if ( keyCheck(VK_LEFT)>0)
		posX -= posX < 2 ? 0 : 1;
	if ( keyCheck(VK_RIGHT)>0)
		posX += posX > consoleWidth-13 ? 0 : 1;

	if (posX>1)
		WriteAt(L" ",posX-1, Console::WindowHeight-4);
	if (posX<consoleWidth-12)
		WriteAt(L" ",posX+11, Console::WindowHeight-4);
	WriteAt(L"▄■■▀▀▀▀▀■■▄",posX, Console::WindowHeight-4);
}

inline Boolean clsPlatform::keyCheck(Int16 keyCode)
{
	return GetAsyncKeyState(keyCode) != 0  && GetConsoleWindow() == GetForegroundWindow() ? true : false;
}