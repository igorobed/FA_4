#pragma once
#include <windows.h>
#include <vector>
#include <string>

extern HWND hListBox, hDlgRed, hDlgGreen, hDlgBlue, hDlgAngles;
extern HINSTANCE hInst;

#define MOVING_TIMER_ID 44
#define ABOUT_INFO 4
#define DO_EXIT 5
#define DRAW_ON_CLICK 6
#define LISTBOX_ID 7
#define BUTTON_ID 8
#define BUTTON2_ID 9
#define EDIT_ID 10
#define DIALOG_BUTTON_ID 11
#define DRAW_RANDOM_RECTANGLES 12

#define BUF_SIZE 500

typedef enum { wait, drawOnClick, moveOneFigure, drawRandomRectangles }progState;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������� �����(������, �������, �� �������)
typedef enum { in, out, border } point_position;

//��������� ������������
double cross_prod(POINT, POINT, POINT);
//��������� ������������
double scalar_prod(POINT, POINT, POINT);
//����� �� ����� �� �����
bool is_point_on_edge(POINT, POINT, POINT);
//������������ �� ����� � �����
bool is_edge_cross(POINT, POINT, POINT, POINT);
//����� ����� �� ����
POINT point_on_ray(POINT, int);
//������������ ����� ������������ ��������������(������/�������/�� �������)
point_position is_point_in_polygon(std::vector<POINT>, POINT, int);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isCorrectColor(int);
bool isCorrectAngles(int);

class Figure
{
public:
    int id_generator;
    int id;
    HPEN hp;
    std::vector<POINT> points;
    //������� ����� ������
    int extremeRight, extremeLeft, extremeTop, extremeBottom;

    Figure();

    int getExtremeRight();
    int getExtremeLeft();
    int getExtremeTop();
    int getExtremeBottom();
    void setRandomBrush();
    void setBrush(int, int, int);
    void addPoint(POINT);
    void draw(HDC);
    void delFirst();
    void clear();
    //����� ���������� ����� ����
    void setAngles(int number, std::vector<POINT>);
    int getNumbAngles();
    //����� ����� �����, ����� �������
    void move(int, int);
    POINT* inFieldsAngle(POINT);

    friend class workSpace;
};

class workSpace
{
public:
    std::vector<POINT> borders;
    std::vector<Figure> tempFigures;
    Figure* currFigure;//��������������(����������) ������, ���� ��� ����, ���� ����� ����, ������ ������� ������ ������������ ������
    workSpace();
    void draw(HDC);
    void add(Figure);
    bool insideBorders(POINT*);
    void clear();
    POINT* nearAngle(POINT*);
    Figure* getCurr();
    void moveCurrFigure(int, int);
    void addRandRectangles(Figure*);
};

void addMenus(HWND);
void addControls(HWND);
void RegisterDialogClassAbout(HINSTANCE);
void displayDialogAbout(HWND);
void RegisterDialogClassEdit(HINSTANCE);
void displayDialogEdit(HWND);

