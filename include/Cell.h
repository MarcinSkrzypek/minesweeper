#ifndef CELL_H
#define CELL_H

#include <Windows.h>
#include "CellState.h"

class Cell {
public:
    Cell(HWND parentHWND, HINSTANCE hInst, int x, int y, int width, int height, int id);
    ~Cell();
    void create();
    HWND getHandle() const;
    int getId() const;
    CellState getState() const;
    void setState(CellState newState);

private:
    HWND hwndButton;
    HWND parentHWND;
    HINSTANCE hInstance;
    int x, y, width, height, id;
    CellState state;
};

#endif // CELL_H
