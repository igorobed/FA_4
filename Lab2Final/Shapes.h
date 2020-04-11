#pragma once
#include <windows.h>
#include <string>
#include <CommCtrl.h>

#define MOVING_TIMER_ID 1
#define STATUS_BAR_TIMER 2

#define M_CIRCLE 10
#define M_RHOMB 11
#define M_TRIANGLE 12
#define M_PENTAGON 13
#define M_BROWNIAN_SHAPE 14
#define M_STOPPING_SHAPE 15
#define M_STOPPING_CHANGES 16

#define HEIGHT_STATUS_BAR 20

enum horDirections
{
    left, right
};

enum verDirections
{
    up, down
};

enum currentShape
{
    nowCircle, nowTriangle, nowRhomb, nowPentagon
};

class Shape
{
public:
    int x;
    int y;
    horDirections hor;
    verDirections ver;
    int horSpeed;
    int verSpeed;
    int horSize = 70;
    int verSize = 70;

    HBRUSH circleBrush;
    LOGBRUSH logbrush;
    HPEN hPen;
    int hPenRand;
    COLORREF color;
    int countRand;
    HRGN rg;

    bool pressBrown;
    bool pressStop;
    bool pressStopChanges;

    int r, g, b;
    int style, hatch;

    Shape();

    void SetStopChanges(bool pressStopChanges);
    bool GetStopChanges();
    void SetBrown(bool pressBrown);
    bool GetBrown();
    void SetStop(bool pressStop);
    bool GetStop();

    void Update(int width, int height);
    void GetProperties(wchar_t* out_w);
    void BrownUpdate();
};

class Triangle : public Shape
{
public:
    POINT pt[3];

    void Print(HDC hdc);
};

class Circle : public Shape
{
public:

    void Print(HDC hdc);
};

class Rhomb : public Shape
{
public:
    POINT pt[4];

    void Print(HDC hdc);
};

class Pentagon : public Shape
{
public:
    POINT pt[5];

    void Print(HDC hdc);
};
