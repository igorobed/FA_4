#include "Header.h"

HWND hEditDay, hEditMonth, hEditYear, hEditHour, hEditMin, hEditSec, hEditMlsec, hInterval, hOut;
boost::posix_time::ptime momentTime;



void fillingFields() 
{
    SYSTEMTIME st;

    GetLocalTime(&st);
    momentTime = boost::posix_time::microsec_clock::local_time();


    char temp_buf[LEN_TEMP_MASS];
    WCHAR w_temp_buf[LEN_TEMP_MASS];
    size_t size = LEN_TEMP_MASS;
    size_t outSize;

    _itoa_s(st.wDay, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditDay, w_temp_buf);

    _itoa_s(st.wMonth, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditMonth, w_temp_buf);

    _itoa_s(st.wYear, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditYear, w_temp_buf);

    _itoa_s(st.wHour, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditHour, w_temp_buf);

    _itoa_s(st.wMinute, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditMin, w_temp_buf);

    _itoa_s(st.wSecond, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditSec, w_temp_buf);

    _itoa_s(st.wMilliseconds, temp_buf, LEN_TEMP_MASS, 10);
    mbstowcs_s(&outSize, w_temp_buf, size, temp_buf, size);
    SetWindowText(hEditMlsec, w_temp_buf);

    SetWindowText(hInterval, L"...");

    displayClock();

}


bool isNumber(char* mas)
{
    char* ptr = mas;
    while (*ptr)
    {
        if (!isdigit(*ptr++))
            return FALSE;
    }
    return TRUE;
}
bool isValidDate(int day, int month, int year)
{
    const int MAX_VALID_YR = 9999;
    const int MIN_VALID_YR = 1800;
    if ((year > MAX_VALID_YR) || (year < MIN_VALID_YR))
    {
        return FALSE;
    }
    if ((month < 1) || (month > 12))
    {
        return FALSE;
    }
    if ((day < 1) || (day > 31))
    {
        return FALSE;
    }
    ////////////
    if (month == 2)
    {
        if (boost::gregorian::gregorian_calendar::is_leap_year(boost::gregorian::greg_year(year)))
            return (day <= 29);
        else
            return (day <= 28);
    }
    ///////////
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return (day <= 30);
    return TRUE;
}
bool isValidInterval(int interval)
{
    if ((interval <= 10000) && (interval >= 50))
    {
        return TRUE;
    }
    else
        return FALSE;
}
bool isValidTime(int hours, int min, int sec, int mlsec)
{
    if ((hours < 0) || (hours > 24))
        return FALSE;
    if ((min < 0) || (min > 60))
        return FALSE;
    if ((sec < 0) || (sec > 60))
        return FALSE;
    if ((mlsec < 0) || (mlsec > 1000))
        return FALSE;
    return TRUE;
}


void displayClock()
{
    std::string tempString = boost::posix_time::to_simple_string(momentTime);
    std::wstring wstr(tempString.begin(), tempString.end());
    LPCWSTR sw = wstr.c_str();
    SetWindowText(hOut, sw);
}


void displayDialog(HWND hwnd)
{
    HWND hdlg = CreateWindowEx(NULL, L"MyDialogClass", L"Œ œ–Œ√–¿ÃÃ≈", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 300, 200,
        hwnd, NULL, NULL, NULL);

    CreateWindowEx(NULL, L"BUTTON", L"«¿ –€“‹", WS_VISIBLE | WS_CHILD, 0, 0, 100, 40, hdlg, (HMENU)DIALOG_CLOSE,
        NULL, NULL);

    CreateWindowEx(NULL, L"STATIC", L"◊¿—€, Œ“Œ¡–¿∆¿ﬁŸ»≈—ﬂ — «¿ƒ¿ÕÕ€Ã »Õ“≈–¬¿ÀŒÃ", WS_VISIBLE | WS_CHILD, 0, 50, 200, 40, hdlg, NULL,
        NULL, NULL);
}
void AddMenus(HWND hwnd)
{
    HMENU hmenu = CreateMenu();
    HMENU underMenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)underMenu, L"Ã≈Õﬁ");

    AppendMenu(underMenu, MF_STRING, SET_CURRENT_TIME, L"“≈ ”Ÿ≈≈ ¬–≈Ãﬂ");
    AppendMenu(underMenu, MF_STRING, VIEW_ABOUT, L"Œ œ–Œ√–¿ÃÃ≈");
    AppendMenu(underMenu, MF_STRING, MENU_EXIT, L"¬€’Œƒ");

    SetMenu(hwnd, hmenu);
}
void AddControls(HWND hwnd)
{
    CreateWindowEx(NULL,
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"œ”— ",      // Button text 
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        200,         // x position 
        20,         // y position 
        100,        // Button width
        30,        // Button height
        hwnd,     // Parent window
        (HMENU)BUTTON_CLICK,       // No menu.
        NULL,
        NULL);

    hInterval = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        325, 10, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"»Õ“≈–¬¿À", WS_VISIBLE | WS_CHILD,
        385, 10, 80, 20, hwnd, NULL, NULL, NULL);


    hOut = CreateWindowEx(NULL, L"STATIC", L"¬–≈Ãﬂ", WS_VISIBLE | WS_CHILD,
        200, 300, 250, 20, hwnd, NULL, NULL, NULL);



    hEditDay = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 10, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"ƒ≈Õ‹", WS_VISIBLE | WS_CHILD,
        70, 10, 50, 20, hwnd, NULL, NULL, NULL);

    hEditMonth = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 40, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Ã≈—ﬂ÷", WS_VISIBLE | WS_CHILD,
        70, 40, 50, 20, hwnd, NULL, NULL, NULL);

    hEditYear = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 70, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"√Œƒ", WS_VISIBLE | WS_CHILD,
        70, 70, 50, 20, hwnd, NULL, NULL, NULL);

    hEditHour = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 100, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"◊¿—", WS_VISIBLE | WS_CHILD,
        70, 100, 50, 20, hwnd, NULL, NULL, NULL);

    hEditMin = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 130, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Ã»Õ", WS_VISIBLE | WS_CHILD,
        70, 130, 50, 20, hwnd, NULL, NULL, NULL);

    hEditSec = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 160, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"—≈ ", WS_VISIBLE | WS_CHILD,
        70, 160, 50, 20, hwnd, NULL, NULL, NULL);

    hEditMlsec = CreateWindowEx(NULL, L"EDIT", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 190, 50, 20, hwnd, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"ÃÀ—≈ ", WS_VISIBLE | WS_CHILD,
        70, 190, 50, 20, hwnd, NULL, NULL, NULL);
}