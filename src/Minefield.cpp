#include "Minefield.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Minefield::Minefield() {
    this->rows = 10;
    this->columns = 10;
    this->numberOfMines = 5;

    field = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        field[i] = new int[columns]{0};
    }

    setMines();
}

Minefield::~Minefield() {
    for(int i = 0; i < rows; ++i) {
        delete[] field[i];
    }
    delete[] field;
}

void Minefield::setMines() {
    srand(time(NULL));
    int minesSet = 0;
    const int total_cells = rows * columns;
    while (minesSet < numberOfMines) {
        int ind = rand() % total_cells;
        int row = ind / columns;
        int col = ind % columns;

        if (field[row][col] != 9) {
            field[row][col] = 9;
            minesSet++;
            updateCellsAround(row, col);
        }
    }
}

void Minefield::updateCellsAround(int row, int col) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (isValidCell(row + i, col + j)) {
                field[row + i][col + j]++;
            }
        }
    }
}

bool Minefield::isValidCell(int row, int col) {
  return row >= 0 && row < rows && col >= 0 && col < columns && field[row][col] != 9;
}

void Minefield::show() {
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Minefield::check(int row, int column) {
    return field[row][column];
}

int Minefield::getNumberOfMines() {
    return numberOfMines;
}

int* Minefield::getMines() {
    return minesOnField;
}

int Minefield::getRows() {
    return rows;
}

int Minefield::getColumns() {
    return columns;
}
