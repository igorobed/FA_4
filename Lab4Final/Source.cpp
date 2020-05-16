#include "Header.h"

Figure tempFigure;
workSpace myWorkSpace;
std::vector<Figure> figuresInList;
POINT* tempPoint = NULL;//всегда кргда этот указатель не равен нулю значит, что мы тянем за угол
                        //во всех остальных случаях он равен нулю
size_t outSize;//чтобы засовывать везде, где требуется

int drawOnClickFlag = 0;
progState state = wait;

POINT cursorPosition;

int tempX = 0;
int tempY = 0;
int tempRed = 0;
int tempGreen = 0;
int tempBlue = 0;
int tempNumb = 3;

char buf_c[BUF_SIZE];
wchar_t buf_w[BUF_SIZE];
std::string temp_s;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    hInst = hInstance;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
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

    RegisterDialogClassAbout(hInstance);
    RegisterDialogClassEdit(hInstance);

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"MyClassName",
        L"LAB",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        50, 50, 900, 615,
        NULL, NULL, hInstance, NULL);

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

//предварительно загрузим необходимый мне курсор
HCURSOR hcur = LoadCursor(hInst, IDC_CROSS);
HCURSOR hcur0 = LoadCursor(hInst, IDC_ARROW);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    switch (msg)
    {
    case WM_CREATE:
        addMenus(hwnd);
        addControls(hwnd);
        SetTimer(hwnd, MOVING_TIMER_ID, 50, NULL);
        break;
    case WM_SETCURSOR:
        if (myWorkSpace.getCurr() == NULL)
        {
            break;
        }
        GetCursorPos(&cursorPosition);
        ScreenToClient(hwnd, &cursorPosition);
        if (is_point_in_polygon(myWorkSpace.getCurr()->points, cursorPosition, 900) == in)
        {
            SetCursor(hcur);
        }
        else
        {
            SetCursor(hcur0);
        }
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ABOUT_INFO:
            displayDialogAbout(hwnd);
            break;
        case DO_EXIT:
            KillTimer(hwnd, MOVING_TIMER_ID);
            PostQuitMessage(0);
            break;
        case DRAW_ON_CLICK:
            state = drawOnClick;
            drawOnClickFlag = 1;
            myWorkSpace.clear();
            tempFigure.clear();
            break;
        case BUTTON_ID://сохранить фигуру в список
            if (state == wait)
            {
                if (myWorkSpace.getCurr() != NULL)
                {
                    figuresInList.push_back(*(myWorkSpace.getCurr()));
                    char buf_c[100];
                    wchar_t buf_w[100];
                    size_t outSize;
                    //std::string temp_s = std::to_string(myWorkSpace.getCurr()->id);
                    std::string temp_s = std::to_string(figuresInList.size());
                    strcpy_s(buf_c, temp_s.c_str());
                    mbstowcs_s(&outSize, buf_w, 100, buf_c, 100);
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buf_w);
                }
            }
            else if (state == drawRandomRectangles)
            {
                //хмммм вытаскиваем из спец массива в воркспейсе 3 прямоугольника и сохраняем их
                for (size_t i = 0; i < 3; i++)
                {
                    figuresInList.push_back(myWorkSpace.tempFigures[i]);
                    //std::string temp_s = std::to_string(myWorkSpace.tempFigures[i].id);
                    std::string temp_s = std::to_string(figuresInList.size());
                    strcpy_s(buf_c, temp_s.c_str());
                    mbstowcs_s(&outSize, buf_w, BUF_SIZE, buf_c, BUF_SIZE);
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buf_w);
                }
                //раз делать с ними все равно ничего нельзя
                myWorkSpace.clear();
                ///в самом конце делаем опять исх. состояние
                state = wait;
            }
            break;
        case BUTTON2_ID://загрузить фигуру из списка
            int uSelectedItem;
            //определяем номер выделенной строки
            uSelectedItem = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0L);
            //если есть выделенный элемент, то записываем его в текущий элемент рабочей области, предварительно ее очистив
            if (uSelectedItem != LB_ERR)
            {
                myWorkSpace.clear();
                myWorkSpace.add(figuresInList[uSelectedItem]);
                break;
            }
            break;
        case DRAW_RANDOM_RECTANGLES:
            state = drawRandomRectangles;
            myWorkSpace.clear();
            tempFigure.clear();
            myWorkSpace.addRandRectangles(&tempFigure);
            break;
        case EDIT_ID:
            if (myWorkSpace.getCurr() == NULL)
            {
                MessageBox(NULL, L"Чтобы редактировать, в рабочем поле должна быть одна фигура", L"Edit error", MB_OK);
            }
            else
            {
                //запускаем диалоговое окно редактор
                displayDialogEdit(hwnd);
            }
            break;
        default:
            break;
        }
    case WM_LBUTTONDOWN:
        GetCursorPos(&cursorPosition);
        ScreenToClient(hwnd, &cursorPosition);
        if (state == drawOnClick && drawOnClickFlag == 1)
        {
            if (myWorkSpace.insideBorders(&cursorPosition))//здесь проверка, что точка внутри рабочего пространства
            {
                tempFigure.addPoint(cursorPosition);
            }
        }
        else if (myWorkSpace.getCurr() != NULL)
        {
            tempPoint = myWorkSpace.nearAngle(&cursorPosition);
            if (myWorkSpace.getCurr() != NULL && is_point_in_polygon(myWorkSpace.getCurr()->points, cursorPosition, 900) == in && tempPoint == NULL)//если мы внутри фигуры
            {
                state = moveOneFigure;
                tempX = cursorPosition.x;
                tempY = cursorPosition.y;
            }
        }
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&cursorPosition);
        ScreenToClient(hwnd, &cursorPosition);
        if (tempPoint != NULL)
        {
            if (myWorkSpace.insideBorders(&cursorPosition))
            {
                tempPoint->x = cursorPosition.x;
                tempPoint->y = cursorPosition.y;
            }
        }
        else if (state == moveOneFigure)
        {
            myWorkSpace.moveCurrFigure(cursorPosition.x - tempX, cursorPosition.y - tempY);
            tempX = cursorPosition.x;
            tempY = cursorPosition.y;
        }
        break;

    case WM_RBUTTONDOWN:
        if (state == drawOnClick && drawOnClickFlag == 1)
        {
            tempFigure.delFirst();
            if (tempFigure.getNumbAngles() < 3)
            {
                MessageBox(NULL, L"Необходимо установить минимум три точки. Начните сначала, опять выбрав соответствующий раздел", L"Edit error", MB_OK);
                state = wait;
                drawOnClickFlag == 0;
                break;
            }
            myWorkSpace.add(tempFigure);
            state = wait;
            drawOnClickFlag == 0;
        }
        break;
    case WM_LBUTTONUP:
        if (tempPoint != NULL)
        {
            tempPoint = NULL;
        }
        else if (state == moveOneFigure)
        {
            state = wait;
        }
        break;
    case WM_TIMER:
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        myWorkSpace.draw(hdc);

        EndPaint(hwnd, &ps);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        KillTimer(hwnd, MOVING_TIMER_ID);
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

LRESULT CALLBACK DlgPrcEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case DIALOG_BUTTON_ID:
            GetWindowText(hDlgRed, buf_w, BUF_SIZE);
            wcstombs_s(&outSize, buf_c, BUF_SIZE, buf_w, BUF_SIZE);
            temp_s = std::string(buf_c);
            tempRed = std::stoi(temp_s, &outSize);
            ////////////////////////
            GetWindowText(hDlgGreen, buf_w, BUF_SIZE);
            wcstombs_s(&outSize, buf_c, BUF_SIZE, buf_w, BUF_SIZE);
            temp_s = std::string(buf_c);
            tempGreen = std::stoi(temp_s, &outSize);
            ///////////////////////
            GetWindowText(hDlgBlue, buf_w, BUF_SIZE);
            wcstombs_s(&outSize, buf_c, BUF_SIZE, buf_w, BUF_SIZE);
            temp_s = std::string(buf_c);
            tempBlue = std::stoi(temp_s, &outSize);
            if (isCorrectColor(tempRed) && isCorrectColor(tempGreen) && isCorrectColor(tempBlue))
            {
                myWorkSpace.getCurr()->setBrush(tempRed, tempGreen, tempBlue);
                tempRed = 0;
                tempGreen = 0;
                tempBlue = 0;
            }
            else
            {
                tempRed = 0;
                tempGreen = 0;
                tempBlue = 0;
                MessageBox(hwnd, L"Введенное неверное значение одного или более. Должно быть числом из множества {0, ..., 255}", NULL, MB_OK);
                break;
            }
            //////////////////////ОБРАБОТКА ЧИСЛА УГЛОВ

            GetWindowText(hDlgAngles, buf_w, BUF_SIZE);
            wcstombs_s(&outSize, buf_c, BUF_SIZE, buf_w, BUF_SIZE);
            temp_s = std::string(buf_c);
            tempNumb = std::stoi(temp_s, &outSize);
            if (isCorrectAngles(tempNumb))
            {
                //если число введенных углов, равно исходному числу углов, то менять ничего не надо
                if (tempNumb == myWorkSpace.getCurr()->getNumbAngles())
                {
                    break;
                }
                //Здесь работает функция привидения фигуру к соответствующему числу углов
                myWorkSpace.getCurr()->setAngles(tempNumb, myWorkSpace.borders);
                tempNumb = 3;
            }
            else
            {
                tempNumb = 3;
                MessageBox(hwnd, L"Некорректное число углов фигуры. Должно быть числом из множества {3, ..., 10}", NULL, MB_OK);
                break;
            }
            break;
        default:
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

void RegisterDialogClassEdit(HINSTANCE hInst)
{
    WNDCLASSEX dialog;

    dialog.cbSize = sizeof(WNDCLASSEX);
    dialog.style = 0;
    dialog.lpfnWndProc = DlgPrcEdit;
    dialog.cbClsExtra = 0;
    dialog.cbWndExtra = 0;
    dialog.hInstance = hInst;
    dialog.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
    dialog.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    dialog.lpszMenuName = NULL;
    dialog.lpszClassName = L"MyDialogClassEdit";
    dialog.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&dialog))
    {
        MessageBox(NULL, L"Dialog Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }
}