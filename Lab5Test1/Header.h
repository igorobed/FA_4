#pragma once
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

extern HINSTANCE hInstGlobal;
extern HWND radioButton1, radioButton2, radioButton3, radioButton4, radioButton5, button; 

#define ID_BUTTON 52
#define ID_ABOUT 12
#define ID_EXIT 13
#define ID_RADIO1 22
#define ID_RADIO2 23
#define ID_RADIO3 24
#define ID_RADIO4 25
#define ID_RADIO5 26


class Object
{
public:
	HWND h;
	int x, y;

	Object(HWND, int, int);
	HWND getHWND();
	int getX();
	int getY();
	int getWidth();
	int getHight();

	bool operator ==(Object);
	bool operator !=(Object);
};

extern vector<Object> objects;

void AddMenus(HWND);
void AddControls(HWND);
void RegisterDialogClassAbout(HINSTANCE);
void displayDialogAbout(HWND);