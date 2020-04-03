#pragma once
#include <windows.h>
#include <stdlib.h>
#include <boost/date_time.hpp>
#include <cctype>

extern HWND hEditDay, hEditMonth, hEditYear, hEditHour, hEditMin, hEditSec, hEditMlsec, hInterval, hOut;
extern boost::posix_time::ptime momentTime;


#define LEN_TEMP_MASS 10
#define MENU_EXIT 22
#define VIEW_ABOUT 44
#define DIALOG_CLOSE 33
#define BUTTON_CLICK 123
#define SET_CURRENT_TIME 100
#define TIMER_ID 1

void AddControls(HWND);
void AddMenus(HWND);
void RegisterDialogClass(HINSTANCE);
void displayDialog(HWND);
void fillingFields();


int valuesForPtime(int*);


bool isValidDate(int, int, int);
bool isValidTime(int, int, int, int);
bool isValidInterval(int);
bool isNumber(char*);


void displayClock();

