#include "Header.h"

bool logical_var = false;//надо сейчас отрисовывать часы или нет(TRUE - надо, FALSE -нет)
int interval = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"MyClassName";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RegisterDialogClass(hInst);

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MyClassName",
        L"CLOCK",
        WS_OVERLAPPEDWINDOW,
        100, 100, 500, 500,
        NULL, NULL, hInst, NULL);

    SetTimer(hwnd, TIMER_ID, 1000, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, ncmdshow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (msg)
    {
    case WM_CREATE:
        AddControls(hwnd);
        AddMenus(hwnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case MENU_EXIT:
            DestroyWindow(hwnd);
            break;
        case VIEW_ABOUT:
            displayDialog(hwnd);
            break;
        case SET_CURRENT_TIME:
            fillingFields();
            break;
        case BUTTON_CLICK:
            int mass_clock[7];//[day, month, year, hour, minute, sec, mlsec]
            int val_error;

            val_error = valuesForPtime(mass_clock);
            if (val_error == 1)
            {
                MessageBox(hwnd, L"Verify that numerical values ​​are entered correctly", L"Error entering numerical value", MB_OK);
                break;
            }

            if (isValidDate(mass_clock[0], mass_clock[1], mass_clock[2]) == false)
            {
                logical_var = false;
                MessageBox(hwnd, L"Check the date is correct", L"Correctness of date", MB_OK);
                break;
            }

            if (isValidTime(mass_clock[3], mass_clock[4], mass_clock[5], mass_clock[6]) == false)
            {
                logical_var = false;
                MessageBox(hwnd, L"Check the time is correct", L"Correctness of time", MB_OK);
                break;
            }

            if (isValidInterval(interval) == false)
            {
                logical_var = false;
                MessageBox(hwnd, L"Correct interval - 50...10000", L"Interval correctness", MB_OK);
                break;
            }

            momentTime = boost::posix_time::ptime{ boost::gregorian::date
                 {
                 boost::gregorian::greg_year(mass_clock[2]),
                 boost::gregorian::greg_month(mass_clock[1]),
                 boost::gregorian::greg_day(mass_clock[0])
                 },
                 boost::posix_time::time_duration{mass_clock[3],mass_clock[4],mass_clock[5]} +boost::posix_time::millisec(mass_clock[6]) };

            SetTimer(hwnd, TIMER_ID, interval, NULL);
            logical_var = true;

            displayClock();
            break;
        }
        break;
    case WM_TIMER:
        //adding millisecs to momentTime
        if (logical_var == true)
        {
            momentTime += boost::posix_time::millisec(interval);
        }
        InvalidateRect(hwnd, NULL, false);
        break;
    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        if (logical_var == true)
        {
            displayClock();
        }
        EndPaint(hwnd, &ps);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK DlgPrc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case DIALOG_CLOSE:
            DestroyWindow(hwnd);
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void RegisterDialogClass(HINSTANCE hInst)
{
    WNDCLASSEX dialog;

    dialog.cbSize = sizeof(WNDCLASSEX);
    dialog.style = 0;
    dialog.lpfnWndProc = DlgPrc;
    dialog.cbClsExtra = 0;
    dialog.cbWndExtra = 0;
    dialog.hInstance = hInst;
    dialog.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
    dialog.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    dialog.lpszMenuName = NULL;
    dialog.lpszClassName = L"MyDialogClass";
    dialog.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&dialog))
    {
        MessageBox(NULL, L"Dialog Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }
}

int valuesForPtime(int* mas)
{
    wchar_t buf_w[LEN_TEMP_MASS];
    char buf_c[LEN_TEMP_MASS];
    size_t size = LEN_TEMP_MASS;
    size_t outSize;

    GetWindowText(hEditDay, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[0] = atoi(buf_c);

    GetWindowText(hEditMonth, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[1] = atoi(buf_c);

    GetWindowText(hEditYear, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[2] = atoi(buf_c);

    GetWindowText(hEditHour, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[3] = atoi(buf_c);

    GetWindowText(hEditMin, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[4] = atoi(buf_c);

    GetWindowText(hEditSec, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[5] = atoi(buf_c);

    GetWindowText(hEditMlsec, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        return 1;
    mas[6] = atoi(buf_c);


    //defoult == 500 mlsec
    GetWindowText(hInterval, buf_w, LEN_TEMP_MASS);
    wcstombs_s(&outSize, buf_c, size, buf_w, size);
    if (isNumber(buf_c) == false)
        interval = 500;
    else
        interval = atoi(buf_c);
    return 0;
}