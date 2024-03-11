#include "Cell.h"

Cell::Cell(HWND parentHWND, HINSTANCE hInst, int x, int y, int width, int height, int id)
    : parentHWND(parentHWND), hInstance(hInst), x(x), y(y), width(width), height(height), id(id), state(CellState::Unrevealed) {
    create();
}

Cell::~Cell() {
    DestroyWindow(hwndButton);
}

void Cell::create() {
    hwndButton = CreateWindowW(L"BUTTON", L"",
                               WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
                               x, y, width, height,
                               parentHWND, (HMENU)id,
                               hInstance, NULL);
}

HWND Cell::getHandle() const {
    return hwndButton;
}

int Cell::getId() const {
    return id;
}

CellState Cell::getState() const {
    return state;
}

void Cell::setState(CellState newState) {
    state = newState;
}
