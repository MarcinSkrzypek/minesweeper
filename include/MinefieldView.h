#ifndef MINEFIELDVIEW_H
#define MINEFIELDVIEW_H

#include "BitmapLoader.h"
#include "Cell.h"
#include "Minefield.h"
#include <vector>

class MinefieldView {
  public:
    MinefieldView(Minefield& minefield, HWND hwnd, HINSTANCE hInst, BitmapLoader& bitmapLoader);
    ~MinefieldView();

    void initialize();
    void handleCellLeftClick(int wmId);
    void handleCellRightClick(HWND hwndControl);
    void revealCell(int i, int j);
    void updateCellOnRightClick(Cell* cell);
    void cascadeReveal(int i, int j);
    void revealAllMines();
    void resetCells(int rows, int cols);

  private:
    Minefield& minefield;
    HWND hwnd;
    HINSTANCE hInst;
    BitmapLoader& bitmapLoader;
    std::vector<std::vector<Cell*>> cells;
    int rows, columns;

    void createCells();
    void releaseCells();
};

#endif // MINEFIELDVIEW_H
