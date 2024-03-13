#include "MinefieldView.h"

MinefieldView::MinefieldView(Minefield& minefield, HWND hwnd, HINSTANCE hInst, BitmapLoader& bitmapLoader)
    : minefield(minefield), hwnd(hwnd), hInst(hInst), bitmapLoader(bitmapLoader), timer(nullptr), rows(minefield.getRows()), columns(minefield.getColumns()) {
}

MinefieldView::~MinefieldView() {
    releaseCells();
}

void MinefieldView::initialize(Timer* timer) {
    this->timer = timer;
    createCells();
}

void MinefieldView::createCells() {
    for (int i = 0; i < rows; ++i) {
        std::vector<Cell*> row;
        for (int j = 0; j < columns; ++j) {
            row.push_back(new Cell(hwnd, hInst, 10 + 32 * j, 54 + 32 * i, 32, 32, 100 + i * columns + j));
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
    if(!GameConfig::getGameOverFlag()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (cells[i][j]->getId() == wmId) {
                    revealCell(i, j);
                    return;
                }
            }
        }
    }
}

void MinefieldView::handleCellRightClick(HWND hwndControl) {
    if(!GameConfig::getGameOverFlag()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (cells[i][j]->getHandle() == hwndControl) {
                    updateCellOnRightClick(cells[i][j]);
                    return;
                }
            }
        }
    }
}

void MinefieldView::updateCellOnRightClick(Cell* cell) {
    HBITMAP bitmap = nullptr;
    switch(cell->getState()) {
    case CellState::Unrevealed:
        bitmap = bitmapLoader.getImage(L"MinesGuess");
        cell->setState(CellState::Guessed);
        decCurrentMinesCount();
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
        incCurrentMinesCount();
        break;
    }

    SendMessage(cell->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
}

void MinefieldView::revealCell(int i, int j) {
    int fieldValue = minefield.check(i, j);

    if(cells[i][j]->getState() != CellState::Revealed) {
        if (fieldValue == 9) {
            revealAllMines();
            GameConfig::setGameOverFlag(true);
            timer->stop();
            MessageBoxW(hwnd, L"You lost.", L"Game over", MB_OK | MB_ICONEXCLAMATION);
        } else if (fieldValue == 0) {
            cascadeReveal(i, j);
        } else {
            HBITMAP bitmap = bitmapLoader.getBitmapForValue(fieldValue);
            if (bitmap != nullptr) {
                SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
                cells[i][j]->setState(CellState::Revealed);
            }
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

void MinefieldView::resetCells(int rows, int cols) {
    this->rows = rows;
    this->columns = cols;

    releaseCells();
    createCells();
}

int MinefieldView::getCurrentMinesCount() {
    return currentMinesCount;
}

void MinefieldView::setCurrentMinesCount(int currentMinesCount) {
    this->currentMinesCount = currentMinesCount;
}

void MinefieldView::incCurrentMinesCount() {
    this->currentMinesCount += 1;
}

void MinefieldView::decCurrentMinesCount() {
    this->currentMinesCount -= 1;
}
