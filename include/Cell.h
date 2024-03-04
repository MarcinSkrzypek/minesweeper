#ifndef CELL_H
#define CELL_H

#include <Windows.h>

class Cell {
public:
    Cell(HWND parentHWND, HINSTANCE hInst, int x, int y, int width, int height, int id);
    ~Cell();
    void create();
    HWND getHandle() const;

private:
    HWND hwndButton;
    HWND parentHWND;
    HINSTANCE hInstance;
    int x, y, width, height, id;
};

#endif // CELL_H
