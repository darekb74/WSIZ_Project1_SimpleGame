#ifndef CLSBALL_H
#define CLSBALL_H
#pragma once

#include "windows.h"
#include <iostream>

using namespace System;

class clsBall
{
public:
	Double posX, posY, werX, werY;
	Int16 mX,mY;
	Double alpha;
	UINT speed, cspeed;
	Double speedUp;
	Boolean ballLost;
public:
	clsBall()
	{
		ballLost = false;
		posX = 60;
		posY = 25;
		speed = 6;
		setColor(15);
		cspeed = speed;
		speedUp = consoleWidth-2;
		
		alpha = M_PI/4;
		werX = cos(alpha);
		werY = sin(alpha);
		mX = 1; // w prawo
		mY = 1; // w dół
	}

	void reset()
	{
		posX = 60;
		posY = 25;
		speed = 6;
		for (UInt16 n=1; n< consoleWidth-1; setColor((n<(consoleWidth-2)*2/3 ? 14 - (n<(consoleWidth-2)/3 ? 2 : 0) : 10) ), WriteAt(L"▒",n++,consoleHeight-2));
		setColor(15);
		cspeed = speed;
		speedUp = consoleWidth-2;
		
		alpha = M_PI/4;
		werX = cos(alpha);
		werY = sin(alpha);
		mX = 1; // w prawo
		mY = 1; // w dół
	}

	inline Int16 getX() { return posX; }
	//inline Int16 getX2() { return werX > 0 ? floor(posX) : ceil(posX); }

	inline Int16 getY() { return posY; }

	void move(Int16 intPalX)
	{
		Double predictX, predictY;
		predictX = posX + werX*mX;
		predictY = posY + werY*mY;

		// sprawdzamy czy nie odbijamy się od ściany
		if (predictX <= 1 || predictX >= consoleWidth-1) 
			mX *=-1; // zmiana kierunku
		if (predictY < 3 )
			mY *= -1; // zmiana kierunku

		// uderzenie w paletkę
		if (predictY >= consoleHeight-4 && (intPalX <= posX && intPalX+ 11 >= posX) ) 
		{
			mY *= -1; // zmiana kierunku
			PlaySoundA((LPCSTR)"Sounds\\3.wav", NULL, SND_FILENAME | SND_ASYNC);
			switch ((Int16)posX - intPalX)
			{
				case 0:
					alpha += (mX < 0 ? -M_PI/9 : 0); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 1:
					alpha += (mX < 0 ? -M_PI/12 : M_PI/36); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 2:
					alpha += (mX < 0 ? -M_PI/18 : M_PI/18); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 3:
					alpha += (mX < 0 ? M_PI/36 : M_PI/12); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX > 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 7:
					alpha += (mX < 0 ? M_PI/12 : -M_PI/36); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 8:
					alpha += (mX < 0 ? M_PI/18 : -M_PI/18); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 9:
					alpha += (mX < 0 ? M_PI/36 : -M_PI/12); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				case 10:
					alpha += (mX < 0 ? 0 : -M_PI/9); // piłka <-- 
					alpha = (alpha < 0.2 ? 0.2 : alpha > M_PI/2 ? M_PI/2 : alpha);
					mX *= (mX < 0 ? -1 : 1);
					werX = cos(alpha);
					werY = sin(alpha); // piłka <-- 
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
				default:
					werX = cos(alpha);
					werY = sin(alpha);
					//WriteAt("werX: " + werX + " werY:" + werY + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
					break;
			}
			werX = (werX > 2 ? 2 : werX);
		}
		//WriteAt("werX: " + werX + " werY:" + werY + " ALPHA: " + alpha + " Punkt uderzenia:" + ((Int16)posX - intPalX) + "  ", 1 , 1);
		if (predictY >= consoleHeight-3 && mY>0 ) 
			{
				PlaySoundA((LPCSTR)"Sounds\\fail.wav", NULL, SND_FILENAME); // reset 
				ballLost = true;
				posX = 60;
				posY = 25;
				speed = 6;
				for (UInt16 n=1; n< consoleWidth-1; setColor((n<(consoleWidth-2)*2/3 ? 14 - (n<(consoleWidth-2)/3 ? 2 : 0) : 10) ), WriteAt(L"▒",n++,consoleHeight-2));
				setColor(15);
				cspeed = speed;
				speedUp = consoleWidth-2;
				alpha = M_PI/4;
				werX = cos(alpha);
				werY = sin(alpha);
				mX = 1; // w prawo
				mY = 1; // w dół
				return;
			}
		posX += werX * mX;
		posY += werY * mY;

		posX = posX > consoleWidth-1 ? consoleWidth-1 : posX;
		posX = posX < 0 ? 0 : posX;
	}
};

#endif