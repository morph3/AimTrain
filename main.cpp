#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "cursor.h"
#include "colors.h"
#include <time.h>
using namespace::std;
struct Target {
	int x1;
	int x2;
	int y1;
	int y2;
};
typedef struct Target;

void Clear(int nScreenWidth, int nScreenHeight, char screen[], WORD screenAttr[]);
void Draw(int nScreenWidth, int nSreenHeight, char screen[], WORD screenAttr[], struct Mine mine);
void GenerateMine(int *randNum, int nScreenWidth, int nScreenHeight);
int main() {
	int nScreenWidth = 70;
	int nScreenHeight = 30;
	string sTitle;

	auto *screen = new char[nScreenWidth*nScreenHeight];
	auto *screenAttr = new WORD[nScreenWidth*nScreenHeight];

	DWORD dwBytesWritten = 0;
	DWORD dwAttrBytesWritten = 0;
	
	SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
	COORD coord = { (short)nScreenWidth, (short)nScreenHeight };

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	SetConsoleWindowInfo(hConsole, TRUE, &m_rectWindow);

	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleActiveScreenBuffer(hConsole);

	m_rectWindow = { 0,0,(short)(nScreenWidth - 1), (short)(nScreenHeight - 1) };
	SetConsoleWindowInfo(hConsole, 1, &m_rectWindow);

	CHAR_INFO* m_bufScreen = new CHAR_INFO[nScreenWidth*nScreenHeight];
	memset(m_bufScreen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);
	
	HINSTANCE hInst = NULL;
	HCURSOR hCurs1, hCurs2;
	
	POINT ptCursor;
	/*
	CreateCursor(hInst , 19, 2, 32,32, ANDmaskCursor, XORmaskCursor);
	WNDCLASS wc;
	wc.hCursor = LoadCursor(hInst , MAKEINTRESOURCE(230));

	*/
	
	char cTitle[50];
	time_t t;

	srand((unsigned) time(&t));
	int randNum = rand() % (nScreenWidth * nScreenHeight);

	Target mine;
	mine.x1 = randNum;
	mine.x2 = mine.x1 + 1;
	mine.y1 = mine.x1 + nScreenWidth;
	mine.y2 = mine.y1 + 1;

	while (true) {
		Sleep(50);
		GetCursorPos(&ptCursor);
		sprintf(cTitle, "X: %d Y: %d ", ptCursor.x, ptCursor.y);
		SetConsoleTitle((sTitle + cTitle).c_str());

		if ((ptCursor.x && ptCursor.y) == (mine.x1 || mine.x2 || mine.y1 || mine.y2)) {
			GenerateMine(&randNum, nScreenWidth, nScreenHeight);
		}
		
		

		Clear(nScreenWidth, nScreenHeight, screen, screenAttr);
		Draw(nScreenWidth, nScreenHeight, screen, screenAttr,mine);


		WriteConsoleOutputAttribute(hConsole, screenAttr, (DWORD)nScreenWidth*nScreenHeight, { 0,0 }, &dwAttrBytesWritten);
		WriteConsoleOutputCharacter(hConsole, screen, (DWORD)nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	system("Pause");
	return 0;
}

void Clear(int nScreenWidth, int nScreenHeight, char screen[], WORD screenAttr[]) {
	//emptying the pixels each time
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) {
		screen[i] = ' ';
		screenAttr[i] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
	}
}


void Draw(int nScreenWidth , int nScreenHeight, char screen[], WORD screenAttr[] , struct Target mine) {
	// draw borders
	for (int i = 0; i < nScreenWidth; i++) {
		screen[i] = ' ';
		screen[(nScreenHeight - 1) * nScreenWidth + i] = ' ';

		screenAttr[i] = FULL_PIXEL_BLOCK;
		screenAttr[(nScreenHeight - 1) * nScreenWidth + i] = FULL_PIXEL_BLOCK;
	}
	screen[mine.x1] = ' ';
	screen[mine.x2] = ' ';
	screen[mine.y1] = ' ';
	screen[mine.y2] = ' ';

	screenAttr[mine.x1] = FULL_PIXEL_BLOCK | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	screenAttr[mine.x2] = FULL_PIXEL_BLOCK | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	screenAttr[mine.y1] = FULL_PIXEL_BLOCK | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	screenAttr[mine.y2] = FULL_PIXEL_BLOCK | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
}

void GenerateMine(int *randNum ,int nScreenWidth , int nScreenHeight) {
	*randNum = rand() % (nScreenWidth * nScreenHeight);
}