#include "stdafx.h"
#include "windows.h"
#include "clsMatrix.h"
#include <fstream>
#include <string>

using namespace std;

extern UInt32 wynik;

clsMatrix::clsMatrix(void)
{
	endLevel = false;
	UInt16 tempArray1[10] = {0,5,2,8,3,3,0,0,0,0};
	UInt16 tempArray2[10] = {0,0,1,0,5,0,0,0,0,9};
    for(UInt16 i = 0; i < 10; tabBValue[i] = tempArray1[i], tabBTrans[i] = tempArray2[i++]);
	loadLevels();
	//tabBricks = gcnew array<Int16, 2>(mLINES, 19); // deklaracja managed 
}

Boolean clsMatrix::loadLevels()
{
	fstream myFile;
	myFile.open("Levels.dat", fstream::in);
	string strT;
    if(!myFile.fail())
    {
		for(UInt16 n=0; n < 10; n++)
		{
			getline(myFile,strT);
			if (strT == "[Level]") 
			{
				getline(myFile,strT);
				dbLevels[n].nextLevel = atoi(strT.c_str()); //nr poziomu
				if (dbLevels[n].nextLevel == -1) break;
				strT = "";
				for (UInt16 i = 0; i < mLINES; getline(myFile,strT),dbLevels[n].lvlDesign += strT, i++);
				dbLevels[n].lvlDesign += "00000000000000000000";
			}
		}
    }
    myFile.close();
	return true;
}

void clsMatrix::showBricks(Int16 intLVL)
{
	// ustawiamy macierz cegiełek
	// 0 - brak
	// 1 - zwykła
	// 2 - podwójna
	// 3 - wybuchająca
	// 4 - niewidoczna 1->2
	// 5 - niewidoczna 2
	// 9 - niezniszczalna
	String^ tmpStr;
	
	Int16 x,y;

	for (y=0;y<mLINES+1;y++)
		for (x=0;x<20;x++)
		{
			tabBricks[y][x] = atoi(dbLevels[intLVL].lvlDesign.substr(y*20+x,1).c_str());
			printBrick(x,y,tabBricks[y][x]);
		}
}

UInt16 clsMatrix::countBricks()
{
	UInt16 x,y, bCount = 0;
	for (y=0;y<mLINES+1;y++)
		for (x=0;x<20;x++)
			if (tabBricks[y][x]>0 && tabBricks[y][x]!=9) bCount++;
	return bCount;
}


void clsMatrix::flashMatrix(UInt16 color)
{
	// ze względu na ubogie możliwości konsoli
	// pozostaje tylko jako opcja - lepiej nie używać :) F2
	std::string spaces(consoleWidth-2, ' ');
	setColor(15+color);
	WriteAt(gcnew String(spaces.c_str()),1,3);
	WriteAt(gcnew String(spaces.c_str()),1,4);
	for (UInt16 n=3+mLINES; n<consoleHeight-3; WriteAt(gcnew String(spaces.c_str()),1,n++));
	Int16 x,y;
	for (y=0;y<mLINES;y++)
		for (x=0;x<20;x++)
				printBrick(x,y,tabBricks[y][x],color);
	return;
}

void clsMatrix::printBrick(Int16 x, Int16 y, Int16 type, UInt16 backColor)
{
	String^ brick;
	switch (type)
	{
	case 1:
		setColor(15+backColor);
		brick = L"▒▒▒▒▒▒";
		break;
	case 2:
		setColor(14+backColor);
		brick = L"▓▓▓▓▓▓";
		break;
	case 3:
		brick = L"░¤░░¤░";
		setColor(12+backColor);
		break;
	case 0:
		setColor(15+backColor);
		brick = L"      ";
		break;
	case 4:
		setColor(0+backColor);
		brick = L"░░░░░░";
		break;
	case 5:
		setColor(11+backColor);
		brick = L"▓▒░░▒▓";
		break;
	case 9:
		setColor(2+backColor);
		brick = L"██████";
		break;
	}
	WriteAt(brick,1+x*6,5+y);
	setColor(15);
}

void clsMatrix::checkMatrix(clsBall *tmpBall)
{
	Double predictX, predictY;


	predictX = tmpBall->posX + tmpBall->werX * tmpBall->mX;
	predictY = tmpBall->posY + tmpBall->werY * tmpBall->mY;

	Int16 mcX = ((Int16)tmpBall->posX-1)/6; //aktualny
	Int16 mX = ((Int16)predictX-1)/6; //przewidywany
	Int16 mcY = ((Int16)tmpBall->posY-5); //aktualny
	Int16 mY = ((Int16)predictY-5); //przewidywany
	if (mY>mLINES-1 || mY<0) return; // nie w polu cegieł -> ponad lub poniżej
	if (mX>19 || mX < 0) return; // nie w polu cegieł -> z prawej lub lewej
		
	// 0 - brak
	// 1 - zwykła
	// 2 - podwójna
	// 3 - wybuchająca
	// 4 - niewidoczna -> 5
	// 5 - eks niewidoczna
	// 9 - niezniszczalna
	switch (tabBricks[mY][mX])
	{
		case 0:
			// pusto - teoretycznie nie uderzamy w żadną cegłę, ale
			// musimy sprawdzić, czy nie mamy doczynienia ze specjalnym
			// przypadkiem, w którym przelatujemy pomiędzy dwoma, złączomnymi
			// rogami cegłami
			if (!changeWer(tmpBall,mX,mY,mcX,mcY,true))
			{
				// przypadek specjalny
				if(tabBricks[mY][mcX]==3)
					brickExplode(mY,mcX);
				else {
					playSound(tabBricks[mY][mcX]);
					wynik += tabBValue[tabBricks[mY][mcX]];
					tabBricks[mY][mcX] = tabBTrans[tabBricks[mY][mcX]];
					printBrick(mcX,mY,tabBricks[mY][mcX]);
				}

				if(tabBricks[mcY][mX]==3)
					brickExplode(mcY,mX);
				else {
					playSound(tabBricks[mcY][mX]);
					wynik += tabBValue[tabBricks[mcY][mX]];
					tabBricks[mcY][mX] = tabBTrans[tabBricks[mcY][mX]];
					printBrick(mX,mcY,tabBricks[mcY][mX]);
				}
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			if (changeWer(tmpBall,mX,mY,mcX,mcY)==true)
			{
				if(tabBricks[mY][mX]==3)
					brickExplode(mY,mX);
				else {
					playSound(tabBricks[mY][mX]);
					wynik += tabBValue[tabBricks[mY][mX]];
					tabBricks[mY][mX] = tabBTrans[tabBricks[mY][mX]];
					printBrick(mX,mY,tabBricks[mY][mX]);
				}
			} else { // przypadek szczególny
				if(tabBricks[mY][mcX]==3)
					brickExplode(mY,mcX);
				else {
					playSound(tabBricks[mY][mcX]);
					wynik += tabBValue[tabBricks[mY][mcX]];
					tabBricks[mY][mcX] = tabBTrans[tabBricks[mY][mcX]];
					printBrick(mcX,mY,tabBricks[mY][mcX]);
				}

				if(tabBricks[mcY][mX]==3)
					brickExplode(mcY,mX);
				else {
					playSound(tabBricks[mcY][mX]);
					wynik += tabBValue[tabBricks[mcY][mX]];
					tabBricks[mcY][mX] = tabBTrans[tabBricks[mcY][mX]];
					printBrick(mX,mcY,tabBricks[mcY][mX]);
				}
			}

			break;
		case 9:
			if (!changeWer(tmpBall,mX,mY,mcX,mcY)) //przypadek szczególny
			{
				if(tabBricks[mY][mcX]==3)

					brickExplode(mY,mcX);
				else {
					playSound(tabBricks[mY][mcX]);
					wynik += tabBValue[tabBricks[mY][mcX]];
					tabBricks[mY][mcX] = tabBTrans[tabBricks[mY][mcX]];
					printBrick(mcX,mY,tabBricks[mY][mcX]);
				}

				if(tabBricks[mcY][mX]==3)
					brickExplode(mcY,mX);
				else {
					playSound(tabBricks[mcY][mX]);
					wynik += tabBValue[tabBricks[mcY][mX]];
					tabBricks[mcY][mX] = tabBTrans[tabBricks[mcY][mX]];
					printBrick(mX,mcY,tabBricks[mcY][mX]);
				}
			} else
				playSound(tabBricks[mY][mX]);
			break;
		default:
			break;
	}
	WriteAt("WYNIK: " + wynik, 52,1);
	if (countBricks() == 0) endLevel = true;
}

void clsMatrix::playSound(UInt16 brickType)
{
	switch(brickType)
	{
		case 1:
			PlaySoundA((LPCSTR)"Sounds\\1.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 2:
			PlaySoundA((LPCSTR)"Sounds\\2.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 3:
			PlaySoundA((LPCSTR)(rand()%2>0 ? "Sounds\\explosion0.wav" : "Sounds\\explosion1.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 4:
			PlaySoundA((LPCSTR)(rand()%2>0 ? "Sounds\\glass0.wav" : "Sounds\\glass1.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 5:
			PlaySoundA((LPCSTR)(rand()%2>0 ? "Sounds\\glass0.wav" : "Sounds\\glass1.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 9:
			PlaySoundA((LPCSTR)"Sounds\\3.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
	}
}

void clsMatrix::brickExplode(UInt16 mY, UInt16 mX)
{
	// najpierw zerujemy cegłę i dodajemy wynik (potrzebne tylko porzy pierwszym wywołaniu)
	wynik += tabBValue[tabBricks[mY][mX]];
	tabBricks[mY][mX] = 0;
	printBrick(mX,mY,tabBricks[mY][mX]);
	// teraz reszta
	UInt16 tmp;
	playSound(3); //explode
	//if (mY < 1 || mX < 1 || mY > mLINES || mX > 18) return;

	for (Int16 i=(mX>0 ? -1 : 0); i<(mX<19?2:1); i++)
	{
		if (mY>0){
			wynik += tabBValue[tabBricks[mY-1][mX+i]];
			tmp = tabBricks[mY-1][mX+i];
			tabBricks[mY-1][mX+i] = 0;
			printBrick(mX+i,mY-1,tabBricks[mY-1][mX+i]);
			if (tmp == 3) brickExplode(mY-1,mX+i); // kolejna eksplozja - rekurencyjne wywołanie funkcji
		}
		wynik += tabBValue[tabBricks[mY][mX+i]];
		tmp = tabBricks[mY][mX+i];
		tabBricks[mY][mX+i] = 0;
		printBrick(mX+i,mY,tabBricks[mY][mX+i]);
		if (tmp == 3) brickExplode(mY,mX+i); // kolejna eksplozja - rekurencyjne wywołanie funkcji
		if (mY<mLINES){
			wynik += tabBValue[tabBricks[mY+1][mX+i]];
			tmp = tabBricks[mY+1][mX+i];
			tabBricks[mY+1][mX+i] = 0;
			printBrick(mX+i,mY+1,tabBricks[mY+1][mX+i]);
			if (tmp == 3) brickExplode(mY+1,mX+i); // kolejna eksplozja - rekurencyjne wywołanie funkcji
		}
	}
}

Boolean clsMatrix::changeWer(clsBall *tmpBall, Int16 mX, Int16 mY, Int16 mcX, Int16 mcY, Boolean justCheck)
{
	if (mcY != mY && !justCheck) // uderzenie w pionie
	{
		tmpBall->mY *= -1;
		if (mcX == mX) // to samo x - uderzenie nastąpiło tylko w pionie -> zmieniamy tylko mY
			return true;
	}
	if (mcX != mX && mcX>=0 && mX>=0 && !justCheck) // uderzenie od boku
	{
		tmpBall->mX *= -1;
		if (mcY == mY) // to samo y - uderzenie nastąpiło tylko z boku -> zmieniamy tylko mX
			return true;
	}
	// Uderzenie obustronne (wersory juz zmienione na przeciwne)
	//WriteAt("[DEBUG] SZCZEGOLNE" + tmpBall->alpha + " " + tmpBall->mX + " " + tmpBall->mY + " ",2,2);
	// szczególne przypadki
	if (tabBricks[mY][mcX] == 0 && tabBricks[mcY][mX] == 0 && !justCheck) //Uderzenie w róg cegły - odbijamy wraz z większym wersorem (powinien się zmienić kąt, pomijamy) 
	{
		if (tmpBall->werX - tmpBall->werY > 0.1)
		{
			tmpBall->mX *= -1; // udbicie w poziomie
			//WriteAt("[DEBUG] KANT ODB W POZ",2,2);
		}
		else if (tmpBall->werY - tmpBall->werX > 0.1)
		{
			tmpBall->mY *= -1; // udbicie w pionie
			//WriteAt("[DEBUG] KANT ODB W PION",2,2);
		}
		else
		{
			// niemal idealny kant - odbicie w pionie i w poziomie
			//WriteAt("[DEBUG] KANT IDEALNY",2,2);
		}
		return true;
	}
	UInt16 iA = ( mY>=0 && mcX>=0 ? tabBricks[mY][mcX] : 0),iB = ( mcY>=0 && mX>=0 ? tabBricks[mcY][mX] : 0);
	if (iA > 0 && iB > 0 ) // uderzenie w cegłę teoretycznie nieosiągalną -> powinny się zbić dwie inne
	{
		//WriteAt("[DEBUG] NIEOSIAGALNA    "+mcX+","+mY+" " +mX+","+mcY+"     ",2,2);
		if (justCheck)
		{
			tmpBall->mY *= -1;
			tmpBall->mX *= -1;
		}
		return false; // NIEOSIAGALNA - specjalne traktowanie
	}
	if (justCheck)	return true; // dalej nie sprawdzamy - nie ma specjalnego przypadku
	if (iA>0) // mur poziomy
	{
		tmpBall->mX *= -1; // odbijamy tylko w pionie
		//WriteAt("[DEBUG] MUR POZIOMY    "+mcX+","+mY+" " +mX+","+mY+"     ",2,2);
	}
	if (iB>0) // mur pionowy
	{
		tmpBall->mY *= -1; // odbijamy tylko w poziomie
		//WriteAt("[DEBUG] MUR POIONOWY   "+mX+","+mcY+" " +mX+","+mY+"     ",2,2);
	}

	return true;
}
