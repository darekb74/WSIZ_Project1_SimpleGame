// WSIZ_Project1_SimpleGame.cpp: Określa punkt wejścia dla aplikacji konsoli.
//


#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <conio.h>
#include "WSIZ_Project1_SimpleGame.h"
#include "clsMatrix.h"
#include "clsPlatform.h"
#include "clsGame.h"
#include "clsBall.h"
#include <ctime>
#include <fstream>
#include <string>



using namespace std;
using namespace System;

// https://msdn.microsoft.com/pl-pl/library/system.console_methods(v=vs.110).aspx


//UInt32 wynik = 0;
Double intT[5] = { 0,0,30,1,0 }; // zmienne tymczasowe potrzebne do roznych rzeczy, np wyswietlanie migajacego napisu pauza
UInt32 wynik = 0;
UInt32 najWynik = 0;

clsBall myBall;
clsPlatform myPlatform;
clsMatrix myBricks;
clsGame myGame;
Boolean pause = false;
Boolean flash = false;

UINT TID[5] = {0,0,0,0,0}; // identyfikatory zegarów

void printLogo(UInt16 x=31, UInt16 y=17, Boolean prnInstr = true)
{
	setColor(12);
	WriteAt(L"                                                             ", x,y-2);
	WriteAt(L"                                                             ", x,y-1);
	WriteAt(L" ██╗    ██╗███████╗██╗███████╗███╗   ██╗ ██████╗ ██╗██████╗  ", x,y);
	setColor(14);
	WriteAt(L" ██║    ██║██╔════╝██║╚══███╔╝████╗  ██║██╔═══██╗██║██╔══██╗ ", x,y+1);
	setColor(15);
	WriteAt(L" ██║ █╗ ██║███████╗██║  ███╔╝ ██╔██╗ ██║██║   ██║██║██║  ██║ ", x,y+2);
	setColor(10);
	WriteAt(L" ██║███╗██║╚════██║██║ ███╔╝  ██║╚██╗██║██║   ██║██║██║  ██║ ", x,y+3);
	setColor(11);
	WriteAt(L" ╚███╔███╔╝███████║██║███████╗██║ ╚████║╚██████╔╝██║██████╔╝ ", x,y+4);
	setColor(9);
	WriteAt(L"  ╚══╝╚══╝ ╚══════╝╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═════╝  ", x,y+5);
	WriteAt(L"                                                             ", x,y+6);
	WriteAt(L"                                                             ", x,y+7);
	if (prnInstr)
	{
		setColor(10);
		WriteAt(L"               ABY ROZPOCZĄĆ WCIŚNIJ ¤ENTER¤", x,y+8);
		WriteAt(L"                       SPACJA - PAUZA", x,y+12);
		WriteAt(L"              STRZAŁKI KURSORA - RUCH PALETKI", x,y+13);
		WriteAt(L"                ESCAPE - WYJŚCIE Z PROGRAMU", x,y+14);
		WriteAt(L"                      F2 - BŁYSKAWICA :)", x,y+15);
		setColor(15);
	}
	setColor(15);
	WriteAt(L"╚",0, consoleHeight-1); // wyswietlenie czegokolwiek w lewym dolnym rogu konsoli 
	WriteAt(L"╗",consoleWidth-1,0); // oraz w prawym górnym zapobiega niebieskim artefaktom O_o 
}

void printGameOver(UInt16 x=24, UInt16 y=26)
{
setColor(12);
WriteAt(L" ██ ▄█▀ ▒█████   ███▄    █  ██▓▓█████  ▄████▄       ▄████  ██▀███ ▓██   ██▓", x,y);
WriteAt(L" ██▄█▒ ▒██▒  ██▒ ██ ▀█   █ ▓██▒▓█   ▀ ▒██▀ ▀█      ██▒ ▀█▒▓██ ▒ ██▒▒██  ██▒", x,y+1);
WriteAt(L"▓███▄░ ▒██░  ██▒▓██  ▀█ ██▒▒██▒▒███   ▒▓█    ▄    ▒██░▄▄▄░▓██ ░▄█ ▒ ▒██ ██░", x,y+2);
WriteAt(L"▓██ █▄ ▒██   ██░▓██▒  █▄██▒░██░▒▓█  ▄ ▒▓▓▄ ▄██▒   ░▓█  ██▓▒██▀▀█▄   ░ ▀██▓░", x,y+3);
WriteAt(L"▒██▒ █▄░ ████▓▒░▒██░   ▓██░░██░░▒████▒▒ ▓███▀ ░   ░▒▓███▀▒░██▓ ▒██▒ ░ ██▒▓░", x,y+4);
WriteAt(L"▒ ▒▒ ▓▒░ ▒░▒░▒░ ░ ▒░   ▒ ▒ ░▓  ░░ ▒░ ░░ ░▒ ▒  ░    ░▒   ▒ ░ ▒▓ ░▒▓░  ██▒▒▒ ", x,y+5);
WriteAt(L"░ ░▒ ▒░  ░ ▒ ▒░ ░ ░░   ░ ▒░ ▒ ░ ░ ░  ░  ░  ▒        ░   ░   ░▒ ░ ▒░▓██ ░▒░ ", x,y+6);
WriteAt(L"░ ░░ ░ ░ ░ ░ ▒     ░   ░ ░  ▒ ░   ░   ░           ░ ░   ░   ░░   ░ ▒ ▒ ░░  ", x,y+7);
WriteAt(L"░  ░       ░ ░           ░  ░     ░  ░░ ░               ░    ░     ░ ░     ", x,y+8);
WriteAt(L"                                      ░                            ░ ░    ", x,y+9);
}

void CLR()
{
	string spaces(consoleWidth-2, ' ');
	for (UInt16 n=3; n<consoleHeight-3; WriteAt(gcnew String(spaces.c_str()),1,n++));
}


void drawGUI(Boolean prnLogo = true) {
	Int16 n;
	string tmp(L'═', consoleWidth-2);
	setColor(15);
	WriteAt(L"╔",0,0);
	WriteAt(L"╚",0, consoleHeight-1);
	WriteAt(L"╗",consoleWidth-1,0);
	WriteAt(L"╝",consoleWidth-1, consoleHeight-1);
	for (n=1; n< consoleWidth-1; WriteAt(L"═",n,0),WriteAt(L"═",n,2),WriteAt(L"═",n,consoleHeight-1), WriteAt(L"═",n++,consoleHeight-3) );
	for (n=1; n< consoleHeight-1; WriteAt(L"║",0,n),WriteAt(L"║",consoleWidth-1,n++));
	WriteAt(L"╠",0,2);
	WriteAt(L"╠",0,consoleHeight-3);
	WriteAt(L"╣",consoleWidth-1,2);
	WriteAt(L"╣",consoleWidth-1,consoleHeight-3);

	WriteAt("PIŁKI: " + myGame.balls + "        ", 3,1);
	WriteAt("WYNIK: " + wynik + "        ", 52,1);
	WriteAt("NAJLEPSZY WYNIK: " + najWynik + "  ", 93,1);
	if (prnLogo) printLogo();
}

inline void WriteAt( String^ s, int x, int y )
{
   try
   {
      Console::SetCursorPosition( x, y );
      Console::Write( s );
   }
   catch ( ArgumentOutOfRangeException^ e ) 
   {
      Console::Clear();
      Console::WriteLine( e->Message );
   }

}

inline void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

inline void consoleCenter()
{
	//Console::SetWindowSize( consoleWidth, consoleHeight );
	//system("pause");
	
	//uchwyt do okna konsoli
	HWND ConsoleWindow;
	ConsoleWindow=GetForegroundWindow();

	//uchwyt i rozmiar ekranu
	HWND   hwndScreen;
	RECT   rectScreen;		
	hwndScreen=GetDesktopWindow ();  
	GetWindowRect(hwndScreen,&rectScreen); 		

	// normalizujemy fonty i wybieramy ich wielkość uwzględniając wielkość ekranu
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	UInt16 fontWidth, fontHeight, tmpX = rectScreen.right, tmpY = rectScreen.bottom;
	
	if (tmpY >= 1080) // 1080+
	{
	fontWidth = 9;
	fontHeight = 19;
	} else if (tmpY >= 1024) // 1024+
	{
		fontWidth = 8;
		fontHeight = 18;
	} else if (tmpY >= 960) // 960+
	{
		fontWidth = 8;
		fontHeight = 17;
	} else if (tmpY >= 900) // 900+
	{
		fontWidth = 8;
		fontHeight = 16;
	} else if (tmpY >= 864) // 864+
	{
		fontWidth = 7;
		fontHeight = 15;
	} else if (tmpY >= 800) // 800+
	{
		fontWidth = 7;
		fontHeight = 14;
	} else if (tmpY >=768) // 768+
	{
		fontWidth = 6;
		fontHeight = 13;
	} else if (tmpY >=720) // 720+
	{
		fontWidth = 6;
		fontHeight = 12;
	} else { // 719-   800x600 
		fontWidth = 5;
		fontHeight = 10;
	}

	cfi.dwFontSize.X = 0; // domyślne (7?)
	cfi.dwFontSize.Y = fontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	// Wymiary konsoli uwzględniające wielkość czcionki
	Int16 Width = fontWidth*consoleWidth+33;
	Int16 Height = fontHeight*consoleHeight+39;
	// centrowanie
	Int16 ConsolePosX = ((rectScreen.right-rectScreen.left)/2 - Width/2 );
	Int16 ConsolePosY = ((rectScreen.bottom-rectScreen.top)/2 - Height/2 );
	
	SetWindowPos(ConsoleWindow, NULL, ConsolePosX, ConsolePosY, Width, Height, SWP_SHOWWINDOW );

}

// kolory
// http://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

inline void setColor(Int16 color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void CALLBACK tmrRuchPaletki(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (pause || !myGame.start) return;
	myPlatform.move();
}

void CALLBACK tmrRuchPilki(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (pause || !myGame.start || myBricks.endLevel) return;
	myBall.cspeed--;
	if (myBall.cspeed==0) 
	{
		setColor(15);
		if (myBall.getX()>0 && myBall.getX()<consoleWidth-1)
			WriteAt(L" ",myBall.getX(), myBall.getY());
		myBricks.checkMatrix(&myBall);
		myBall.move(myPlatform.posX);
		if (myBall.getX()>0 && myBall.getX()<consoleWidth-1)
			WriteAt(L"■",myBall.getX(), myBall.getY());
			//WriteAt(L"°",myBall.getX(), myBall.getY());
		myBall.cspeed = myBall.speed;
	}
}


inline Boolean isWindowActive()
{
	return GetConsoleWindow() == GetForegroundWindow();
}

void CALLBACK tmrKontrolaKlawiatury(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	//if (GetAsyncKeyState(VK_F1) !=0 && isWindowActive()) myBricks.endLevel = true;

	if (GetAsyncKeyState(VK_F2) !=0 && isWindowActive()) flash = true;
	if (myBricks.endLevel) return;
	if (GetAsyncKeyState(VK_SPACE) !=0 && isWindowActive()) pause = (pause ? false : true);
	SHORT tmp = GetAsyncKeyState(VK_RETURN);
	if (tmp !=0 && !myGame.start && isWindowActive()) {
		CLR();
		myGame.gameStart(&myBricks, myGame.level);
		drawGUI(false);
	}
	if (pause) return;
}

void CALLBACK tmrObslugaEkranu(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if(flash == true && myGame.start) // flash
	{
		intT[4]++;
		switch ((UInt16)intT[4])
		{
			case 1:
			case 4:
				myBricks.flashMatrix(128);
				break;
			case 7:
				myBricks.flashMatrix(112);
				break;
			case 2:
			case 6:
			case 10:
				myBricks.flashMatrix(0);
				break;
			case 12:
				PlaySoundA((LPCSTR)"Sounds\\thunder0.wav", NULL, SND_FILENAME | SND_ASYNC);
				flash = false;
				intT[4]=0;
				break;
		}
	}
	if (myBall.ballLost == true)
	{
		myBall.ballLost = false;
		WriteAt("PIŁKI: " + --myGame.balls, 3,1);
		if(myGame.balls==0)
		{
			printGameOver();
			PlaySoundA((LPCSTR)"Sounds\\game_over.wav", NULL, SND_FILENAME);
			CLR();
			drawGUI();
			myGame.start = false;
			myGame.level = 0;
			intT[1]=0;
			intT[2]=30;
			intT[3]=1;
			if (wynik > najWynik) najWynik = wynik;
			WriteAt("NAJLEPSZY WYNIK: " + najWynik, 93,1);
		}

	}
	if (myBricks.endLevel) 
	{
		PlaySoundA((LPCSTR)"Sounds\\end_level.wav", NULL, SND_FILENAME);
		CLR();
		myGame.level = myBricks.dbLevels[myGame.level].nextLevel;
		myBricks.showBricks(myGame.level);
		myBricks.endLevel = false;
		myBall.reset();
		return;
	}
	if (!myGame.start) {
		intT[0] = intT[0] >= 15 ? 0 : intT[0]+1;
		setColor(tabCol[(Int16)intT[0]]);
		WriteAt(L"ABY ROZPOCZĄĆ WCIŚNIJ ¤ENTER¤", 46,25);
		// ruch logo
		intT[1] += 0.27;
		intT[2] += intT[3]>0 ? 1 : -1;
		if(intT[1] >= 2*M_PI) intT[1]=0;
		if(intT[2] >= consoleWidth-63) intT[3]=0;
		if(intT[2] <=2 ) intT[3]=1;
		printLogo(intT[2], 17- abs(sin(intT[1]))*7, false);
		return;
	}
	if (pause) {
		intT[0] = intT[0] >= 15 ? 0 : intT[0]+1;
		setColor(tabCol[(Int16)intT[0]]);
		WriteAt("P A U Z A", 56, 40);
		return;
	} else if (intT[0] != 0) {
		setColor(15);
		intT[0] = 0;
		WriteAt("         ", 56, 40);
	}
	// Przyspieszenie
	UInt16 tmp = ceil(myBall.speedUp);
	myBall.speedUp -= 0.1;
	if(myBall.speed > 2 && myBall.speedUp<0)
	{
		PlaySoundA((LPCSTR)"Sounds\\speed_up.wav", NULL, SND_FILENAME | SND_ASYNC);
		myBall.speedUp = consoleWidth-2;
		for (UInt16 n=1; n< consoleWidth-1; setColor((n<(consoleWidth-2)*2/3 ? 14 - (n<(consoleWidth-2)/3 ? 2 : 0) : 10) ), WriteAt(L"░",n++,consoleHeight-2));
		setColor(15);
		myBall.speed -= 1;
	}
	if(ceil(myBall.speedUp) != tmp && tmp!=0 && myBall.speed !=2)
		WriteAt(L" ", tmp,consoleHeight-2);
	WriteAt(L"╚",0, consoleHeight-1); // wyswietlenie czegokolwiek w lewym dolnym rogu konsoli zapobiega niebieskim artefaktom O_o 
	WriteAt(L"╗",consoleWidth-1,0);
	//WriteAt(L"║",0,consoleHeight-4); // kosmetyka - aktualnie zbędna
	//WriteAt(L"║",consoleWidth-1,consoleHeight-4);
	//WriteAt("DO ZBICIA POZOSTALO: " + myBricks.bricksCount+ " ", 89,1);
}

int _tmain(int argc, _TCHAR* argv[])
{
   srand(time(NULL));
   // wczytaj najlepszy wynik
   fstream myFile;
   myFile.open("Highscore.dat", fstream::in );
   string strT0,strT1;
   if(!myFile.fail())
   {
	   getline(myFile,strT0);
	   if (strT0 == "[High Score]") 
	   {
		   getline(myFile,strT0);
		   getline(myFile,strT1);
		   int a = atoi(strT0.c_str());
		   int b = atoi(strT1.c_str()) ^ MAGIC_NUMBER;
		   if (a == b)
		   {
			   najWynik = atoi(strT0.c_str());
		   }
	   }
	   myFile.close();
   }
   consoleCenter();
   //for (int n=0; n<256; setColor(n), cout << n << ":" << (char)n++ << " " );
   //system("pause");
   ShowConsoleCursor(false); //wyłącz kursor
   drawGUI();
   myBall.reset(); // dopiero po inicjalizacji konsoli
   
   TID[0] = SetTimer(NULL, 1, 1, &tmrRuchPilki);
   TID[1] = SetTimer(NULL, 2, myPlatform.speed, &tmrRuchPaletki);
   TID[2] = SetTimer(NULL, 3, 100, &tmrKontrolaKlawiatury);
   TID[3] = SetTimer(NULL, 4, 100, &tmrObslugaEkranu);

   // String^ strPaletka = L"▄■■▀▀▀▀▀■■▄";

   MSG Msg;

   while (true) //niekończąca się pętla
    {
        GetMessage(&Msg, NULL, 0, 0); // obsługa wiadomości
        DispatchMessage(&Msg);        // obsługa wiadomości

		if (GetAsyncKeyState(VK_ESCAPE) !=0 && isWindowActive()) // ESC - wyjście
		{
			pause = true;
			if (MessageBox(NULL, L"Czy na pewno chcesz zakończyć grę?", L"Zakończ grę.", MB_YESNO) == IDYES)
				break;
			else
				pause = false;
		}
    }

	for (int n=1; n < 5; KillTimer(NULL, TID[n++]));
	// zapisz najlepszy wynik
	ofstream myFile2;
	myFile2.open("Highscore.dat");

	if(!myFile.fail())
	{
		myFile2 << "[High Score]\n" << najWynik << "\n" << (najWynik ^ MAGIC_NUMBER) << "\n";
		myFile2.close();
    }
	
	return 0;
}


