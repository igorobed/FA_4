#include "Shapes.h"
#pragma comment(lib,"ComCtl32.Lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);

int timerId, timer2Id;
int width, height;//current width and height main window
RECT windowRect;
HWND hStatus;// for status bar
currentShape whatShape;// what shape view

Triangle triangle;
Circle circle;
Rhomb rhomb;
Pentagon pentagon;

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

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MyClassName",
        L"LAB2",
        WS_OVERLAPPEDWINDOW,
        100, 100, 500, 500,
        NULL, NULL, hInst, NULL);

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

    switch (msg)
    {
    case WM_CREATE:

        AddMenus(hwnd);

        hStatus = CreateWindowEx(NULL, STATUSCLASSNAME, L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 0, 0, hwnd, NULL, NULL, NULL);

        SendMessage(hStatus, SB_SETMINHEIGHT, HEIGHT_STATUS_BAR, 0);//set the height status bar

        //setting the initial data

        timerId = SetTimer(hwnd, MOVING_TIMER_ID, 10, NULL);//creating a timer for drawing shapes
        timer2Id = SetTimer(hwnd, STATUS_BAR_TIMER, 2000, NULL);//creating a timer for rendering the field status bar
        GetWindowRect(hwnd, &windowRect);   
        width = windowRect.right - windowRect.left;//the width of the working zone
        height = windowRect.bottom - windowRect.top;//the height of the working zone
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case M_CIRCLE:
            whatShape = nowCircle;
            break;
        case M_RHOMB:
            whatShape = nowRhomb;
            break;
        case M_TRIANGLE:
            whatShape = nowTriangle;
            break;
        case M_PENTAGON:
            whatShape = nowPentagon;
            break;
        case M_BROWNIAN_SHAPE:
            if (whatShape == nowTriangle)
            {
                if (!triangle.GetBrown())
                    triangle.SetBrown(true);
                else
                    triangle.SetBrown(false);
            }
            else if (whatShape == nowCircle)
            {
                if (!circle.GetBrown())
                    circle.SetBrown(true);
                else
                    circle.SetBrown(false);
            }
            else if (whatShape == nowRhomb)
            {
                if (!rhomb.GetBrown())
                    rhomb.SetBrown(true);
                else
                    rhomb.SetBrown(false);
            }
            else if (whatShape == nowPentagon)
            {
                if (!pentagon.GetBrown())
                    pentagon.SetBrown(true);
                else
                    pentagon.SetBrown(false);
            }
            break;
        case M_STOPPING_SHAPE:
            if (whatShape == nowTriangle)
            {
                if (!triangle.GetStop())
                    triangle.SetStop(true);
                else
                    triangle.SetStop(false);
            }
            else if (whatShape == nowCircle)
            {
                if (!circle.GetStop())
                    circle.SetStop(true);
                else
                    circle.SetStop(false);
            }
            else if (whatShape == nowRhomb)
            {
                if (!rhomb.GetStop())
                    rhomb.SetStop(true);
                else
                    rhomb.SetStop(false);
            }
            else if (whatShape == nowPentagon)
            {
                if (!pentagon.GetStop())
                    pentagon.SetStop(true);
                else
                    pentagon.SetStop(false);
            }
            break;
        case M_STOPPING_CHANGES:
            if (whatShape == nowTriangle)
            {
                if (!triangle.GetStopChanges())
                    triangle.SetStopChanges(true);
                else
                    triangle.SetStopChanges(false);
            }
            else if (whatShape == nowCircle)
            {
                if (!circle.GetStopChanges())
                    circle.SetStopChanges(true);
                else
                    circle.SetStopChanges(false);
            }
            else if (whatShape == nowRhomb)
            {
                if (!rhomb.GetStopChanges())
                    rhomb.SetStopChanges(true);
                else
                    rhomb.SetStopChanges(false);
            }
            else if (whatShape == nowPentagon)
            {
                if (!pentagon.GetStopChanges())
                    pentagon.SetStopChanges(true);
                else
                    pentagon.SetStopChanges(false);
            }
            break;
        }
        break;
    case WM_SIZE:
        SendMessage(hStatus, WM_SIZE, 0, 0);//for status bar

        width = LOWORD(lParam);
        height = HIWORD(lParam) - HEIGHT_STATUS_BAR;
        return 0;
    case WM_TIMER:
        if (wParam == timerId)
        {
            if (whatShape == nowTriangle)
            {
                triangle.Update(width, height);
            }
            else if (whatShape == nowCircle)
            {
                circle.Update(width, height);
            }
            else if (whatShape == nowRhomb)
            {
                rhomb.Update(width, height);
            }
            else if (whatShape == nowPentagon)
            {
                pentagon.Update(width, height);
            }
        }
        if (wParam == timer2Id)
        {
            wchar_t str[100];
            if (whatShape == nowTriangle)
            {
                triangle.GetProperties(str);
                SendMessage(hStatus, SB_SETTEXT, NULL, (LPARAM)str);
            }
            else if (whatShape == nowCircle)
            {
                circle.GetProperties(str);
                SendMessage(hStatus, SB_SETTEXT, NULL, (LPARAM)str);
            }
            else if (whatShape == nowRhomb)
            {
                rhomb.GetProperties(str);
                SendMessage(hStatus, SB_SETTEXT, NULL, (LPARAM)str);
            }
            else if (whatShape == nowPentagon)
            {
                pentagon.GetProperties(str);
                SendMessage(hStatus, SB_SETTEXT, NULL, (LPARAM)str);
            }
        }

        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (whatShape == nowTriangle)
        {
            triangle.Print(hdc);
        }
        else if (whatShape == nowCircle)
        {
            circle.Print(hdc);
        }
        else if (whatShape == nowRhomb)
        {
            rhomb.Print(hdc);
        }
        else if (whatShape == nowPentagon)
        {
            pentagon.Print(hdc);
        }
        EndPaint(hwnd, &ps);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        KillTimer(hwnd, MOVING_TIMER_ID);
        KillTimer(hwnd, STATUS_BAR_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void AddMenus(HWND hwnd)
{
    HMENU hmenu = CreateMenu();
    HMENU underMenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)underMenu, L"MENU");

    AppendMenu(underMenu, MF_STRING, M_CIRCLE, L"CIRCLE");
    AppendMenu(underMenu, MF_STRING, M_TRIANGLE, L"TRIANGLE");
    AppendMenu(underMenu, MF_STRING, M_RHOMB, L"RHOMB");
    AppendMenu(underMenu, MF_STRING, M_PENTAGON, L"PENTAGON");
    AppendMenu(underMenu, MF_STRING, M_BROWNIAN_SHAPE, L"BROWNIAN SHAPE");
    AppendMenu(underMenu, MF_STRING, M_STOPPING_SHAPE, L"STOPPING SHAPE");
    AppendMenu(underMenu, MF_STRING, M_STOPPING_CHANGES, L"STOPPING_CHANGES");

    SetMenu(hwnd, hmenu);
}