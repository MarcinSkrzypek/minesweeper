#include "MinefieldView.h"

MinefieldView::MinefieldView(Minefield& minefield, HWND hwnd, HINSTANCE hInst, BitmapLoader& bitmapLoader)
    : minefield(minefield), hwnd(hwnd), hInst(hInst), bitmapLoader(bitmapLoader), rows(minefield.getRows()), columns(minefield.getColumns()) {
}

MinefieldView::~MinefieldView() {
    releaseCells();
}

void MinefieldView::initialize() {
    bitmapLoader.loadImages();
    createCells();
}

void MinefieldView::createCells() {
    for (int i = 0; i < rows; ++i) {
        std::vector<Cell*> row;
        for (int j = 0; j < columns; ++j) {
            row.push_back(new Cell(hwnd, hInst, 8 + 32 * j, 56 + 32 * i, 32, 32, 100 + i * columns + j));
        }
        cells.push_back(std::move(row));
    }
}

void MinefieldView::releaseCells() {
    for (auto& row : cells) {
        for (auto* cell : row) {
            delete cell;
        }
    }
    cells.clear();
}

void MinefieldView::handleCellLeftClick(int wmId) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (cells[i][j]->getId() == wmId) {
                revealCell(i, j);
                return;
            }
        }
    }
}

void MinefieldView::handleCellRightClick(HWND hwndControl)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if (cells[i][j]->getHandle() == hwndControl)
            {
                updateCellOnRightClick(cells[i][j]);
                return;
            }
        }
    }
}

void MinefieldView::updateCellOnRightClick(Cell* cell) {
    HBITMAP bitmap = nullptr;
    switch(cell->getState())
    {
    case CellState::Unrevealed:
        bitmap = bitmapLoader.getImage(L"MinesGuess");
        cell->setState(CellState::Guessed);
        break;
    case CellState::Revealed:
        return;
    case CellState::Questioned:
        bitmap = NULL;
        cell->setState(CellState::Unrevealed);
        break;
    case CellState::Guessed:
        bitmap = bitmapLoader.getImage(L"QuestionMark");
        cell->setState(CellState::Questioned);
        break;
    }

    SendMessage(cell->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
}

void MinefieldView::revealCell(int i, int j)
{
    int fieldValue = minefield.check(i, j);

    if (fieldValue == 9)
    {
        revealAllMines();
    }
    else if (fieldValue == 0)
    {
        cascadeReveal(i, j);
    }
    else
    {
        HBITMAP bitmap = bitmapLoader.getBitmapForValue(fieldValue);
        if (bitmap != nullptr)
        {
            SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            cells[i][j]->setState(CellState::Revealed);
        }
    }

}

void MinefieldView::cascadeReveal(int i, int j) {
    if (cells[i][j]->getState() == CellState::Revealed) {
        return;
    }

    HBITMAP bitmap = bitmapLoader.getBitmapForValue(minefield.check(i, j));
    if (bitmap != nullptr) {
        SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
        cells[i][j]->setState(CellState::Revealed);
    }

    if (minefield.check(i, j) != 0) {
        return;
    }

    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int newI = i + di;
            int newJ = j + dj;

            if (di == 0 && dj == 0) continue;

            if (minefield.isValidCell(newI, newJ)) {
                if (cells[newI][newJ]->getState() != CellState::Revealed) {
                    cascadeReveal(newI, newJ);
                }
            }
        }
    }
}

void MinefieldView::revealAllMines() {
    auto minePositions = minefield.getMinePositions();

    for (int i = 0; i < minefield.getNumberOfMines(); ++i) {
        int row = minePositions[i].first;
        int col = minePositions[i].second;

        HBITMAP bitmap = bitmapLoader.getBitmapForValue(9);
        if (bitmap != nullptr) {
            SendMessage(cells[row][col]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            cells[row][col]->setState(CellState::Revealed);
        }
    }
}
