#pragma once
#include "Shapes.h"

Shape::Shape()
{
    x = y = 10;
    hor = right;
    ver = down;
    horSpeed = 1;
    verSpeed = 2;
    horSize = verSize = 70;
    color = RGB(r = 128, g = 255, b = 0);
    countRand = 0;
    hPenRand = 0;

    pressBrown = false;
    pressStop = false;
    pressStopChanges = false;
}
void Shape::SetStopChanges(bool pressStopChanges)
{
    this->pressStopChanges = pressStopChanges;
}
bool Shape::GetStopChanges()
{
    return pressStopChanges;
}
void Shape::SetBrown(bool pressBrown)
{
    this->pressBrown = pressBrown;
}
bool Shape::GetBrown()
{
    return pressBrown;
}
void Shape::SetStop(bool pressStop)
{
    this->pressStop = pressStop;
}
bool Shape::GetStop()
{
    return pressStop;
}

void Shape::Update(int width, int height)
{
    if (((countRand % 25) == 0) && ((rand() % 2) == 1) && (pressBrown == true))
    {
        BrownUpdate();
    }

    if (countRand == 50)
    {
        if (pressStopChanges == false)
        {
            if (rand() % 2)
            {
                color = RGB(r = rand() % 256, g = rand() % 256, b = rand() % 256);
                logbrush.lbColor = color;
            }
            if (rand() % 2)
            {
                hPenRand = rand() % 4;
            }
            if (rand() % 2)
            {
                style = rand() % 3;
                logbrush.lbStyle = style;
            }
            if (rand() % 2)
            {
                hatch = rand() % 6;
                logbrush.lbHatch = hatch;
            }
        }
        countRand = 0;
    }
    else
        countRand += 1;

    if (pressStop)
    {
        return;
    }

    if (hor == right)
    {
        x += horSpeed;
        if (x + horSize >= width)
        {
            x = width - horSize;
            hor = left;         
        }
    }
    else if (hor == left)
    {
        x -= horSpeed;
        if (x <= 0) 
        {
            x = 0;
            hor = right;
        }
    }

    if (ver == up)
    {
        y -= verSpeed;
        if (y <= 0)
        {
            y = 0;
            ver = down;
        }
    }
    else if (ver == down)
    {
        y += verSpeed;
        if (y + verSize >= height)
        {
            y = height - verSize;
            ver = up;
        }
    }
}
void Shape::BrownUpdate()
{
    if (rand() % 2)
        horSpeed = rand() % 10;
    if (rand() % 2) //horizontally
    {
        hor = right;
    }
    else
    {
        hor = left;
    }
    if (rand() % 2)
        verSpeed = rand() % 10;
    if (rand() % 2) //vertically
    {
        ver = up;
    }
    else
    {
        ver = down;

    }
}
void Shape::GetProperties(wchar_t* out_w)
{
    char out_c[100];
    std::string str = "RGB(";
    std::string temp_string;
    char temp_c[10];
    _itoa_s(r, temp_c, 10, 10);
    temp_string = temp_c;
    str += temp_string;
    str += std::string(",");
    _itoa_s(g, temp_c, 10, 10);
    temp_string = temp_c;
    str += temp_string;
    str += std::string(",");
    _itoa_s(b, temp_c, 10, 10);
    temp_string = temp_c;
    str += temp_string;
    str += std::string(")");

    str += std::string("   pen style: ");
    switch (hPenRand)
    {
    case 0:
        str += std::string("solid   ");
        break;
    case 1:
        str += std::string("dash   ");
        break;
    case 2:
        str += std::string("dot   ");
        break;
    case 3:
        str += std::string("dashdot   ");
        break;
    default:
        str += std::string("___   ");
        break;
    }

    str += std::string("brush style: ");
    switch (style)
    {
    case 0:
        str += std::string("1   ");
        break;
    case 1:
        str += std::string("2   ");
        break;
    case 2:
        str += std::string("3   ");
        break;
    default:
        str += std::string("___   ");
        break;
    }

    str += std::string("hatch: ");
    switch (hatch)
    {
    case 0:
        str += std::string("1   ");
        break;
    case 1:
        str += std::string("2   ");
        break;
    case 2:
        str += std::string("3   ");
        break;
    case 3:
        str += std::string("4   ");
        break;
    case 4:
        str += std::string("5   ");
        break;
    case 5:
        str += std::string("6   ");
        break;
    default:
        str += std::string("___   ");
        break;
    }

    strcpy_s(out_c, str.c_str());
    size_t outSize;
    mbstowcs_s(&outSize, out_w, 100, out_c, 100);

    return;
}


void Triangle::Print(HDC hdc)
{
    pt[0].x = x + horSize / 2;
    pt[0].y = y;
    pt[1].x = x;
    pt[1].y = y + verSize;
    pt[2].x = x + horSize;
    pt[2].y = y + verSize;

    circleBrush = CreateBrushIndirect(&logbrush);
    hPen = CreatePen(hPenRand, 1, RGB(255, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, circleBrush);

    Polygon(hdc, pt, 3);

    DeleteObject(hPen);
    DeleteObject(circleBrush);
}

//////////////////////////////////////////////////////////////////////////

void Circle::Print(HDC hdc)
{
    circleBrush = CreateBrushIndirect(&logbrush);
    hPen = CreatePen(hPenRand, 1, RGB(255, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, circleBrush);

    Ellipse(hdc, x, y, x + horSize, y + verSize);

    DeleteObject(hPen);
    DeleteObject(circleBrush);
}

/////////////////////////////////////////////////////////////////////////

void Rhomb::Print(HDC hdc)
{
    pt[0].x = x + horSize / 2;
    pt[0].y = y;
    pt[1].x = x;
    pt[1].y = y + verSize / 2;
    pt[2].x = x + horSize / 2;
    pt[2].y = y + verSize;
    pt[3].x = x + horSize;
    pt[3].y = y + verSize / 2;

    circleBrush = CreateBrushIndirect(&logbrush);
    hPen = CreatePen(hPenRand, 1, RGB(255, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, circleBrush);

    Polygon(hdc, pt, 4);

    DeleteObject(hPen);
    DeleteObject(circleBrush);
}

////////////////////////////////////////////////////////////////////////

void Pentagon::Print(HDC hdc)
{
    pt[4].x = x + horSize / 2;
    pt[4].y = y;
    pt[0].x = x;
    pt[0].y = y + verSize / 2;
    pt[1].x = x;
    pt[1].y = y + verSize;
    pt[2].x = x + horSize;
    pt[2].y = y + verSize;
    pt[3].x = x + horSize;
    pt[3].y = y + verSize / 2;

    circleBrush = CreateBrushIndirect(&logbrush);
    hPen = CreatePen(hPenRand, 1, RGB(255, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, circleBrush);

    Polygon(hdc, pt, 5);

    DeleteObject(hPen);
    DeleteObject(circleBrush);
}