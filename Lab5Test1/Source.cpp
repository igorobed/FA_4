#include "Header.h"

bool addObject = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool mv = false;
bool sz = false;
int movingObjIndex;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    hInstGlobal = hInst;

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

    RegisterDialogClassAbout(hInst);

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MyClassName",
        L"LAB",
        WS_OVERLAPPEDWINDOW, 
        100, 100, 900, 600,
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
        case ID_ABOUT:
            displayDialogAbout(hwnd);
            break;
        case ID_EXIT:
            PostQuitMessage(0);
            break;
        case ID_BUTTON:
            MessageBox(NULL, L"Щелкните ЛКМ в месте создания нового элемента", L"Инструкция", MB_OK | MB_ICONASTERISK | MB_DEFAULT_DESKTOP_ONLY);
            addObject = true;
            break;
        default:
            break;
        }

    case WM_PAINT:
        UpdateWindow(hwnd);
        break;

    case WM_LBUTTONDOWN:
        if (addObject)
        {
            for (int i = 0; i < 5; i++)
            {
                if (BST_CHECKED == SendMessage(objects[i].getHWND(), BM_GETCHECK, 0, 0))
                {
                    wstring str;
                    switch (i) 
                    {
                    case 0:
                        str = L"button";
                        break;

                    case 1:
                        str = L"edit";
                        break;

                    case 2:
                        str = L"STATIC";
                        break;

                    case 3:
                        str = L"COMBOBOX";
                        break;

                    case 4:
                        str = L"SCROLLBAR";
                        break;
                    default:
                        break;
                    }

                    POINT p;
                    GetCursorPos(&p);
                    ScreenToClient(hwnd, &p);
                    bool isCorrect = true;
                    for (Object o : objects)
                    {
                        int width = o.getWidth();
                        int hight = o.getHight();
                        int x = o.getX();
                        int y = o.getY();
                        if ((p.x + width >= x && p.x <= x + width) && (p.y + hight >= y && p.y <= y + hight))//если попадает в область
                        {
                            isCorrect = false;
                            break;
                        }
                    }
                    if (isCorrect)
                    {   
                        objects.push_back(Object(CreateWindowEx(NULL, (str.c_str()), (str.c_str()), WS_CHILD | WS_VISIBLE | WS_DISABLED,
                            p.x, p.y, 120, 30, hwnd, (HMENU)0, hInstGlobal, NULL), p.x, p.y));
                        SetWindowText(objects[objects.size() - 1].getHWND(), L"HELLOHELLOHELLO");
                        addObject = false;
                    }
                    break;
                }
            }
        }
        break;
    case WM_MBUTTONDOWN:
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);

        bool isFind = false;
        for (int i = 0; i < objects.size(); ++i)
        {
            Object o = objects[i];
            //увеличивать и уменьшать можн тольк за левый нижний угол объекта
            if ((p.x >= (o.getX() + o.getWidth() - 5) && p.x <= (o.getX() + o.getWidth() + 5)) &&
                (p.y >= (o.getY() + o.getHight() - 5) && p.y <= (o.getY() + o.getHight() + 5)))
            {
                sz = true;
                movingObjIndex = i;
                break;
            }
        }
        break;
    }
    case WM_MBUTTONUP: 
    {
        sz = false;
        break;
    }
    case WM_RBUTTONDOWN:
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hwnd, &p);

        bool isFind = false;
        for (int i = 0; i < objects.size(); ++i) 
        {
            Object o = objects[i];
            if ((p.x >= o.getX() && p.x <= o.getX() + o.getWidth()) &&
                (p.y >= o.getY() && p.y <= o.getY() + o.getHight())) 
            {

                mv = true;
                movingObjIndex = i;
                break;
            }
        }
        break;
    }
    case WM_RBUTTONUP:
    {
        mv = false;
        break;
    }
    case WM_MOUSEMOVE:
    {
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);

        int windWidth = rcClient.right - rcClient.left;
        int windHight = rcClient.bottom - rcClient.top;
        if (sz)
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);

            bool isCollide = false;
            int dh = -1 * (objects[movingObjIndex].getY() - p.y);
            int dw = -1 * (objects[movingObjIndex].getX() - p.x);

           

            for (int i = 0; i < objects.size(); ++i)
            {
                Object o = objects[i];
                if (o != objects[movingObjIndex])
                {
                    int width = o.getWidth();
                    int hight = o.getHight();
                    int x = o.getX();
                    int y = o.getY();

                    if (((objects[movingObjIndex].getX() + dw >= x && objects[movingObjIndex].getX() <= x + width) &&
                        (objects[movingObjIndex].getY() + dh >= y && objects[movingObjIndex].getY() <= y + hight)) ||
                        (objects[movingObjIndex].getX() + dw >= windWidth) || (objects[movingObjIndex].getY() + dh >= windHight))
                    {
                        isCollide = true;
                        break;
                    }
                }
            }

            if (!isCollide && (dw > 10) && (dh > 10))
            {
                MoveWindow(objects[movingObjIndex].getHWND(), objects[movingObjIndex].getX(), objects[movingObjIndex].getY(), dw, dh, TRUE);
            }
        }

        if (mv)
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);
            bool isCollide = false;
            for (int i = 0; i < objects.size(); ++i)
            {

                Object o = objects[i];
                if (o != objects[movingObjIndex])
                {
                    int width = o.getWidth();
                    int hight = o.getHight();
                    int h = objects[movingObjIndex].getHight();
                    int w = objects[movingObjIndex].getWidth();
                    int x = o.getX();
                    int y = o.getY();
                    if (((p.x + w >= x && p.x <= x + width) &&
                        (p.y + h >= y && p.y <= y + hight)
                        ))
                    {
                        isCollide = true;
                        break;
                    }
                }
            }
            if ((p.x + objects[movingObjIndex].getWidth() >= windWidth) || (p.y + objects[movingObjIndex].getHight() >= windHight))
            {
                isCollide = true;
            }
            if (!isCollide)
            {
                MoveWindow(objects[movingObjIndex].getHWND(), p.x, p.y,
                    objects[movingObjIndex].getWidth(), objects[movingObjIndex].getHight(), TRUE);
                objects[movingObjIndex].x = p.x;
                objects[movingObjIndex].y = p.y;
            }
        }
        

        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK DlgPrcAbout(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void RegisterDialogClassAbout(HINSTANCE hInst)
{
    WNDCLASSEX dialog;

    dialog.cbSize = sizeof(WNDCLASSEX);
    dialog.style = 0;
    dialog.lpfnWndProc = DlgPrcAbout;
    dialog.cbClsExtra = 0;
    dialog.cbWndExtra = 0;
    dialog.hInstance = hInst;
    dialog.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
    dialog.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    dialog.lpszMenuName = NULL;
    dialog.lpszClassName = L"MyDialogClassAbout";
    dialog.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&dialog))
    {
        MessageBox(NULL, L"Dialog Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }
}

