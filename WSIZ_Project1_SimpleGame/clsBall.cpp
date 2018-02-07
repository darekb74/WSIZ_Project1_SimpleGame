#include "stdafx.h"
#include "windows.h"
#include <iostream>

#include "WSIZ_Project1_SimpleGame.h"
#include "clsBall.h"

clsBall::clsBall()
{
	posX = 60;
	posY = 20;
	speed = 6;
	for (UInt16 n=1; n< consoleWidth-1; setColor((n<(consoleWidth-2)*2/3 ? 14 - (n<(consoleWidth-2)/3 ? 2 : 0) : 10) ), WriteAt(L"▒",n++,consoleHeight-2));
	setColor(15);
	cspeed = speed;
	speedUp = consoleWidth-2;
		
	if (USE_ALPHA)
	{
		alpha = M_PI/4;
		werX = cos(alpha);
		werY = sin(alpha);
		mX = 1; // w lewo
		mY = 1; // w dół
	} else {
		werX = 1;
		werY = 1;
	}
}

inline void clsBall::setSpeed(UINT speed_, UINT cspeed_, Double speedUp_)
{
	speed = speed_;
	cspeed = cspeed_;
	speedUp = speedUp_;
}

inline Int16 clsBall::getX()
{
	return posX;
}

inline Int16 clsBall::getX2()
{
	return werX > 0 ? floor(posX) : ceil(posX);
}

inline Int16 clsBall::getY()
{
	return posY;
}

void clsBall::move(Int16 intPalX)
{
	Double predictX, predictY;
	if (USE_ALPHA) {
		predictX = posX + werX*mX;
		predictY = posY + werY*mY;
	} else {
		predictX = posX + werX/2;
		predictY = posY + werY/2;
	}

	// sprawdzamy czy nie odbijamy się od ściany
	if (predictX <= 1 || predictX >= consoleWidth-2) 
		if (USE_ALPHA)
			mX *=-1; // zmiana kierunku
		else
			werX *=-1; // zmiana wersora
	if (predictY < 3 )
		if (USE_ALPHA)
			mY *= -1; // zmiana kierunku
		else
			werY *=-1; // zmiana wersora

	// uderzenie w paletkę
	if (predictY >= consoleHeight-4 && (intPalX <= posX && intPalX+ 11 >= posX) ) 
	{
		// (▒z▒lewej) (z▒prawej)
		// 1110000000 0000000111
		// 0000000000 0000000000
		// 0520000250 0520000250
		if (USE_ALPHA)
			mY *= -1; // zmiana kierunku
		else
			werY *=-1; // zmiana wersora - odbicie od paletki
		PlaySoundA((LPCSTR)"Sounds\\3.wav", NULL, SND_FILENAME | SND_ASYNC);
		switch ((Int16)posX - intPalX)
		{
			case 0:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? -M_PI/9 : 0); // piłka <-- 
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? -0.8 : 0); // piłka <-- 
					werX *= (werX > 0 ? -1 : 1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 1:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? -M_PI/12 : M_PI/36); // piłka <-- 
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? -0.6 : -0.2);
					werX *= (werX > 0 ? -1 : 1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 2:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? -M_PI/18 : M_PI/18); // piłka <-- 
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? -0.4 : -0.4);
					werX *= (werX > 0 ? -1 : 1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 3:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? M_PI/36 : M_PI/12); // piłka <-- 
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? -0.2 : -0.6);
					werX *= (werX > 0 ? -1 : 1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 7:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? M_PI/12 : -M_PI/36); // piłka <-- 
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? 0.6 : 0.2);
					werX *= (werX > 0 ? 1 : -1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 8:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? M_PI/18 : -M_PI/18); // piłka <-- 
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? 0.4 : 0.4);
					werX *= (werX > 0 ? 1 : -1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 9:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? M_PI/36 : -M_PI/12); // piłka <-- 
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? 0.2 : 0.6);
					werX *= (werX > 0 ? 1 : -1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			case 10:
				if (USE_ALPHA) {
					alpha += (mX < 0 ? 0 : -M_PI/9); // piłka <-- 
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
				} else {
					werX += (werX < 0 ? 0 : 0.8); // piłka <- 
					werX *= (werX > 0 ? 1 : -1);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
			default:
				if (USE_ALPHA) {
					werX = cos(alpha);
					werY = sin(alpha);
				}
				//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
				break;
		}
		werX = (werX > 2 ? 2 : werX);
	}
	//WriteAt("werX: " + werX + " werY:" + werY + (USE_ALPHA ? " ALPHA: " + alpha : "") + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
	if (predictY >= consoleHeight-3 && (USE_ALPHA ? mY>0 : werY > 0) ) 
		{
			Console::Beep(); // reset 
			posX = 60;
			posY = 20;
			speed = 6;
			for (UInt16 n=1; n< consoleWidth-1; setColor((n<(consoleWidth-2)*2/3 ? 14 - (n<(consoleWidth-2)/3 ? 2 : 0) : 10) ), WriteAt(L"░",n++,consoleHeight-2));
			setColor(15);
			cspeed = speed;
			speedUp = consoleWidth-2;
	
			if (USE_ALPHA)
			{
				speed = 6;
				cspeed = speed;
				alpha = M_PI/4;
				werX = cos(alpha);
				werY = sin(alpha);
				mX = 1; // w prawo
				mY = 1; // w dół
			} else {
				werX = 1;
				werY = 1;
			}
			return;
		}
	if (USE_ALPHA)
	{
		posX += werX * mX;
		posY += werY * mY;
	} else {
		posX += werX/2;
		posY += werY/2;
	}
	posX = posX > consoleWidth-2 ? consoleWidth-2 : posX;
	posX = posX < 0 ? 0 : posX;
}

