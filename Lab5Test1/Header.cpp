#include "Header.h"

HINSTANCE hInstGlobal;
HWND radioButton1, radioButton2, radioButton3, radioButton4, radioButton5, button;

vector<Object> objects;

Object::Object(HWND hwnd, int x, int y)
{
	this->h = hwnd;
	this->x = x;
	this->y = y;
}

HWND Object::getHWND() 
{
	return this->h;
}

int Object::getX() 
{
	return this->x;
}

int Object::getY() 
{
	return this->y;
}

int Object::getWidth() 
{
	RECT rect = { 0 };
	GetWindowRect(this->h, &rect);
	return  rect.right - rect.left;
}

int Object::getHight() 
{
	RECT rect = { 0 };
	GetWindowRect(this->h, &rect);
	return rect.bottom - rect.top;
}

bool Object::operator ==(Object r) 
{
	if (this->h == r.getHWND())
	{
		return true;
	}
	return false;
}

bool Object::operator !=(Object r) 
{
	return !(r == *this);
}

void AddMenus(HWND hwnd)
{
    HMENU hmenu = CreateMenu();
    HMENU underMenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)underMenu, L"MENU");

    AppendMenu(underMenu, MF_STRING, ID_ABOUT, L"ABOUT");
    AppendMenu(underMenu, MF_STRING, ID_EXIT, L"EXIT");

    SetMenu(hwnd, hmenu);
}

void AddControls(HWND hwnd)
{

    button = CreateWindowEx(NULL, L"BUTTON", L"создать", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 160, 120, 20, hwnd,
        (HMENU)ID_BUTTON, NULL, NULL);
    
    radioButton1 = CreateWindowEx(NULL, L"BUTTON", L"button", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        10, 10, 120, 30, hwnd, (HMENU)ID_RADIO1, hInstGlobal, NULL);
    objects.push_back(Object(radioButton1, 10, 10));
    radioButton2 = CreateWindowEx(NULL, L"BUTTON", L"edit", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        10, 40, 120, 30, hwnd, (HMENU)ID_RADIO2, hInstGlobal, NULL);
    objects.push_back(Object(radioButton2, 10, 40));
    radioButton3 = CreateWindowEx(NULL, L"BUTTON", L"label", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        10, 70, 120, 30, hwnd, (HMENU)ID_RADIO3, hInstGlobal, NULL);
    objects.push_back(Object(radioButton3, 10, 70));
    radioButton4 = CreateWindowEx(NULL, L"BUTTON", L"combobox", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        10, 100, 120, 30, hwnd, (HMENU)ID_RADIO4, hInstGlobal, NULL);
    objects.push_back(Object(radioButton4, 10, 100));
    radioButton5 = CreateWindowEx(NULL, L"BUTTON", L"spinbutton", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        10, 130, 120, 30, hwnd, (HMENU)ID_RADIO5, hInstGlobal, NULL);
    objects.push_back(Object(radioButton5, 10, 130));
    
    objects.push_back(Object(button, 10, 160));

}

void displayDialogAbout(HWND hwnd)
{
    HWND hdlg = CreateWindowEx(NULL, L"MyDialogClassAbout", L"О программе", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 400, 400, 500, 160,
        hwnd, NULL, NULL, NULL);
    const wchar_t* text =
        L"Данная программа позволяет создавать в окне обекты следующих типов:\n\
Button, edit, label, comboBox, spinButton.\n\
Созданные объекты можно перемещать по окну и изменять их размеры.";
    CreateWindowEx(NULL, L"STATIC", text, WS_VISIBLE | WS_CHILD, 0, 0, 500, 160, hdlg, NULL, NULL, NULL);
    return;
}