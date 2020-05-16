#include "Header.h"
HWND hListBox, hDlgRed, hDlgGreen, hDlgBlue, hDlgAngles;
HINSTANCE hInst;

Figure::Figure()
{
    hp = CreatePen(0, 3, RGB(0, 0, 0));
    id_generator = 0;
    id = 0;
    extremeBottom = 0;
    extremeTop = 0;
    extremeRight = 0;
    extremeLeft = 0;
}

int Figure::getExtremeRight()
{
    int maxX = 0;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i].x > maxX)
            maxX = points[i].x;
    }
    return maxX;
}

int Figure::getExtremeLeft()
{
    int minX = 900;//правая граница рабочей области
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i].x < minX)
            minX = points[i].x;
    }
    return minX;
}

int Figure::getExtremeTop()
{
    int minY = 615;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i].y < minY)
            minY = points[i].y;
    }
    return minY;
}

int Figure::getExtremeBottom()
{
    int maxY = 0;
    for (size_t i = 0; i < points.size(); i++)
    {
        if (points[i].y > maxY)
            maxY = points[i].y;
    }
    return maxY;
}

void Figure::setRandomBrush()
{
    hp = CreatePen(0, 3, RGB(rand() % 255, rand() % 255, rand() % 255));

}

void Figure::setBrush(int R, int G, int B)
{
    hp = CreatePen(0, 3, RGB(R, G, B));
}

void Figure::addPoint(POINT p)
{
    points.push_back(p);
}

void Figure::delFirst()
{
    points.erase(points.begin());
    extremeTop = getExtremeTop();
    extremeBottom = getExtremeBottom();
    extremeRight = getExtremeRight();
    extremeLeft = getExtremeLeft();
}

void Figure::clear()
{
    id_generator++;
    id = id_generator;
    points.clear();
}

void Figure::draw(HDC hdc)
{
    SelectObject(hdc, hp);
    Polygon(hdc, points.data(), points.size());
}

void Figure::move(int x, int y)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        points[i].x += x;
        points[i].y += y;
    }
}

POINT* Figure::inFieldsAngle(POINT cursor)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        if ((cursor.x > (points[i].x - 5)) && (cursor.x < (points[i].x + 5)) && (cursor.y > (points[i].y - 5)) && (cursor.y < (points[i].y + 5)))
            return &(points[i]);
    }
    return NULL;
}

void Figure::setAngles(int count, std::vector<POINT> borders)
{
    int firstX = points[0].x;
    int firstY = points[0].y;
    points.clear();

    points.push_back(POINT{ firstX, firstY });

    int maxXCoord = borders[3].x;
    int maxYCoord = borders[1].y;


    for (size_t i = 1; i < count; i++)
    {
        POINT p;
        p.x = rand() % maxXCoord;
        p.y = rand() % maxYCoord;
        addPoint(p);
    }
}

int Figure::getNumbAngles()
{
    return points.size();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
workSpace::workSpace()
{
    currFigure = NULL;
    borders.push_back(POINT{ 0, 0 });
    borders.push_back(POINT{ 0, 550 });
    borders.push_back(POINT{ 500, 550 });
    borders.push_back(POINT{ 500, 0 });
}

void workSpace::draw(HDC hdc)
{
    MoveToEx(hdc, borders[0].x, borders[0].y, NULL);
    LineTo(hdc, borders[1].x, borders[1].y);
    LineTo(hdc, borders[2].x, borders[2].y);
    LineTo(hdc, borders[3].x, borders[3].y);
    LineTo(hdc, borders[0].x, borders[0].y);
    for (size_t i = 0; i < tempFigures.size(); i++)
    {
        tempFigures[i].draw(hdc);
    }
    return;
}

void workSpace::add(Figure tempF)
{
    tempFigures.push_back(tempF);
    currFigure = &(tempFigures[tempFigures.size() - 1]);
}

bool workSpace::insideBorders(POINT* ptrCursor)
{
    return (ptrCursor->x >= borders[0].x && ptrCursor->x <= borders[2].x && ptrCursor->y >= borders[0].y && ptrCursor->y <= borders[1].y);
}

void workSpace::clear()
{
    tempFigures.clear();

    currFigure = NULL;
    return;
}

POINT* workSpace::nearAngle(POINT* ptrCursor)
{
    for (size_t i = 0; i < currFigure->points.size(); i++)
    {
        if ((ptrCursor->x > (currFigure->points[i].x - 5)) && (ptrCursor->x < (currFigure->points[i].x + 5)) &&
            (ptrCursor->y > (currFigure->points[i].y - 5)) && (ptrCursor->y < (currFigure->points[i].y + 5)))
            return &(currFigure->points[i]);
    }
    return NULL;
}

Figure* workSpace::getCurr()
{
    return currFigure;
}

void workSpace::moveCurrFigure(int x, int y)
{
    if (currFigure != NULL)
    {
        int tempLeft = currFigure->extremeLeft + x;
        int tempRight = currFigure->extremeRight + x;
        int tempTop = currFigure->extremeTop + y;
        int tempBottom = currFigure->extremeBottom + y;
        if ((tempLeft <= borders[0].x) || (tempRight >= borders[3].x) || (tempTop <= borders[0].y) || (tempBottom >= borders[1].y))
        {
            return;
        }
        for (size_t i = 0; i < currFigure->points.size(); i++)
        {
            currFigure->points[i].x += x;
            currFigure->points[i].y += y;
            //обновление крайних точек внутри фигуры
            currFigure->extremeBottom = currFigure->getExtremeBottom();
            currFigure->extremeTop = currFigure->getExtremeTop();
            currFigure->extremeRight = currFigure->getExtremeRight();
            currFigure->extremeLeft = currFigure->getExtremeLeft();
        }
    }
}

void workSpace::addRandRectangles(Figure* temp)
{
    int randHeight, randWidth;
    //1
    randHeight = rand() % 200 + 40;
    randWidth = rand() % 200 + 40;
    temp->addPoint(POINT{ 10,10 });
    temp->addPoint(POINT{ 10, 10 + randHeight });
    temp->addPoint(POINT{ 10 + randWidth, 10 + randHeight });
    temp->addPoint(POINT{ 10 + randWidth, 10 });
    tempFigures.push_back(*temp);
    temp->clear();
    //2
    randHeight = rand() % 200 + 40;
    randWidth = rand() % 200 + 40;
    temp->addPoint(POINT{ 260, 10 });
    temp->addPoint(POINT{ 260, 10 + randHeight });
    temp->addPoint(POINT{ 260 + randWidth, 10 + randHeight });
    temp->addPoint(POINT{ 260 + randWidth, 10 });
    tempFigures.push_back(*temp);
    temp->clear();
    //3
    randHeight = rand() % 250 + 40;
    randWidth = rand() % 450 + 40;
    temp->addPoint(POINT{ 10, 260 });
    temp->addPoint(POINT{ 10, 260 + randHeight });
    temp->addPoint(POINT{ 10 + randWidth, 260 + randHeight });
    temp->addPoint(POINT{ 10 + randWidth, 260 });
    tempFigures.push_back(*temp);
    temp->clear();
}

void addMenus(HWND hwnd)
{
    HMENU hmenu = CreateMenu();
    HMENU underMenu = CreateMenu();
    HMENU underMenu2 = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)underMenu, L"Общее");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)underMenu2, L"Создание/Редактирование");

    AppendMenu(underMenu, MF_STRING, ABOUT_INFO, L"О программе");
    AppendMenu(underMenu, MF_STRING, DO_EXIT, L"Выход");

    AppendMenu(underMenu2, MF_STRING, DRAW_ON_CLICK, L"Рисовать по щелчку");
    AppendMenu(underMenu2, MF_STRING, DRAW_RANDOM_RECTANGLES, L"Рисовать случайные прямоугольники");
    AppendMenu(underMenu2, MF_STRING, EDIT_ID, L"Редактировать");

    SetMenu(hwnd, hmenu);
    return;
}

void addControls(HWND hwnd)
{
    hListBox = CreateWindowEx(NULL, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD ^ LBS_SORT |
        LBS_WANTKEYBOARDINPUT, 520, 10, 280, 300,
        hwnd, (HMENU)LISTBOX_ID, hInst, NULL);
    //теперь делаю пока тольк одну кнопку, которая сохраняет фигуру если она тольк одна в рабочей области и такой фигуры еще не существует
    CreateWindowEx(NULL, L"BUTTON", L"Сохранить", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 520, 300, 100, 20, hwnd,
        (HMENU)BUTTON_ID, NULL, NULL);

    CreateWindowEx(NULL, L"BUTTON", L"Загрузить", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 700, 300, 100, 20, hwnd,
        (HMENU)BUTTON2_ID, NULL, NULL);
}
//////////////////диалоговые окна///////////////////////////////
///1
void displayDialogAbout(HWND hwnd)
{
    HWND hdlg = CreateWindowEx(NULL, L"MyDialogClassAbout", L"О программе", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 400, 400, 500, 160,
        hwnd, NULL, NULL, NULL);
    const wchar_t* text =
        L"Программа дает возможность простого создания и редактирования фигур.\n\
Есть возможность задать произвольную фигуру щелчками мышки в местах расположения углов.\n\
Есть возможность создать серию случайных прямоугольников.\n\
Сохраненные фигуры записываются в ListBox.\n\
Одну выбранную фигуру можно редактировать.";
    CreateWindowEx(NULL, L"STATIC", text, WS_VISIBLE | WS_CHILD, 0, 0, 500, 160, hdlg, NULL, NULL, NULL);
    return;
}
////2
void displayDialogEdit(HWND hwnd)
{
    HWND hdlg = CreateWindowEx(NULL, L"MyDialogClassEdit", L"Редактирование", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 300, 300,
        hwnd, NULL, NULL, NULL);
    hDlgRed = CreateWindowEx(NULL, L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 0, 100, 20, hdlg, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Красный", WS_VISIBLE | WS_CHILD, 120, 0, 150, 20, hdlg, NULL, NULL, NULL);
    hDlgGreen = CreateWindowEx(NULL, L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 30, 100, 20, hdlg, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Зеленый", WS_VISIBLE | WS_CHILD, 120, 30, 150, 20, hdlg, NULL, NULL, NULL);
    hDlgBlue = CreateWindowEx(NULL, L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 60, 100, 20, hdlg, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Синий", WS_VISIBLE | WS_CHILD, 120, 60, 150, 20, hdlg, NULL, NULL, NULL);
    hDlgAngles = CreateWindowEx(NULL, L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER,
        10, 90, 100, 20, hdlg, NULL, NULL, NULL);
    CreateWindowEx(NULL, L"STATIC", L"Число углов", WS_VISIBLE | WS_CHILD, 120, 90, 150, 20, hdlg, NULL, NULL, NULL);

    CreateWindowEx(NULL, L"BUTTON", L"Изменить", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 120, 100, 20, hdlg, (HMENU)DIALOG_BUTTON_ID, NULL, NULL);
}
///////////////////////////диалоговые окна///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//векторное произведение
double cross_prod(POINT a, POINT b, POINT c)
{
    double x1 = b.x - a.x, y1 = b.y - a.y,
        x2 = c.x - a.x, y2 = c.y - a.y;
    return (x1 * y2 - x2 * y1);
}
//скалярное произведение
double scalar_prod(POINT a, POINT b, POINT c)
{
    double x1 = b.x - a.x, y1 = b.y - a.y,
        x2 = c.x - a.x, y2 = c.y - a.y;
    return (x1 * x2 + y1 * y2);
}
//точка c лежит на ребре ab, если она лежит на одной прямой с ab(векторное произведение = 0),
//и c лежит между a и b(скалярное произведение <= 0; = 0, когда c совпадает с a или c b)
bool is_point_on_edge(POINT a, POINT b, POINT c)
{
    double prod1 = cross_prod(a, b, c),
        prod2 = scalar_prod(c, a, b);
    return (prod1 == 0 && prod2 <= 0) ? true : false;
}
//пересекаются ли отрезки ab и cd
bool is_edge_cross(POINT a, POINT b, POINT c, POINT d)
{
    double prod1, prod2, prod3, prod4;
    prod1 = cross_prod(a, b, c);
    prod2 = cross_prod(a, b, d);
    prod3 = cross_prod(d, c, a);
    prod4 = cross_prod(d, c, b);
    //отрезки ab и cd пересекаются, если c и d лежат по разные стороны от ab,
    //a и b лежат по разные стороны от cd
    bool is_cross = (prod1 * prod2 < 0 && prod3 * prod4 < 0);
    //надо проверить, не принадлежат ли концы одного отрезка другому
    //в этом случае одно(или несколько) векторных произведений обратятся в ноль
    bool cond1, cond2, cond3, cond4, cond1_1, cond2_1, cond3_1, cond4_1, is_one_inside_other;
    cond1 = is_point_on_edge(a, b, c);
    cond2 = is_point_on_edge(a, b, d);
    cond3 = is_point_on_edge(c, d, a);
    cond4 = is_point_on_edge(c, d, b);
    //проверка, что только один конец одного из отрезков принадлежит другому
    //таким образом, из рассмотрения исключаюся ребра, лежащие на луче
    cond1_1 = (cond1 && !(cond2 || cond3 || cond4));
    cond2_1 = (cond2 && !(cond1 || cond3 || cond4));
    cond3_1 = (cond3 && !(cond1 || cond2 || cond4));
    cond4_1 = (cond4 && !(cond1 || cond2 || cond3));
    is_one_inside_other = (cond1_1 || cond2_1 || cond3_1 || cond4_1);

    return (is_cross || is_one_inside_other) ? true : false;
}
//выбор точки на луче, проходящем через проверяемую точку point; в качестве такой точки
//выбрана крайняя точка окна, лежащая на одной горизонтальной прямой с point
POINT point_on_ray(POINT point, int scr_width)
{
    POINT ray{ scr_width, point.y };
    return ray;
}
//положение точки относительно многоугольника
point_position is_point_in_polygon(std::vector<POINT> polygon, POINT begin, int scr_width)
{
    POINT first, second; //вершины многоугольника
    //получение второй точки, лежащей на луче
    POINT end = point_on_ray(begin, scr_width);
    unsigned count = 0; //кол-во пересечений с лучом
    if (polygon.empty())
        return out; //не было передано ни одной вершины
    if (polygon.size() == 1) //была передана одна вершина
        return ((polygon[0].x == begin.x && polygon[0].y == begin.y) ? border : out);
    if (polygon.size() == 2) //был передан отрезок
    { 
        second.x = polygon[1].x; second.y = polygon[1].y;
        first.x = polygon[0].x; first.y = polygon[0].y;
        return (is_point_on_edge(first, second, begin) ? border : out);
    }
    //был передан по крайней мере треугольник
    for (unsigned i = 1; i <= polygon.size(); i++)
    {
        if (i < polygon.size())
        {
            second.x = polygon[i].x; second.y = polygon[i].y;
            first.x = polygon[i - 1].x; first.y = polygon[i - 1].y;
        }
        else
        { //обрабатываем последнее ребро(последняя и первая вершины)
            second.x = polygon[0].x; second.y = polygon[0].y;
            first.x = polygon.back().x; first.y = polygon.back().y;
        }
        //принадлежит ли точка ребру
        if (is_point_on_edge(first, second, begin))
            return border; //точка на границе
        //пересекается ли ребро с лучом(при этом исключаются ребра, лежащие на луче)
        if (is_edge_cross(first, second, begin, end))
            //определяем нижнюю вершину по y
            //ребро не лежит на одной прямой с горизонтальным лучом (из предыдущего условия)
            //если выбрать не горизонтальный луч, надо дополнительно рассмотреть горизонтальные ребра
            if ((first.y > second.y && second.y < begin.y) ||
                (first.y < second.y && first.y < begin.y))
                count++; //нижний конец ребра ниже луча
    }
    if (count % 2)
        return in; //нечетное кол-во пересечений с горизонтальным лучом
    return out;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isCorrectColor(int temp)
{
    return ((temp >= 0) && (temp <= 255));
}

bool isCorrectAngles(int temp)
{
    return ((temp >= 3) && (temp <= 10));
}