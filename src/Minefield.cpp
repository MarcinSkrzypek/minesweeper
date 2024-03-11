#include "Minefield.h"
#include <iostream>

Minefield::Minefield() {
    this->rows = 9;
    this->columns = 9;
    this->numberOfMines = 10;
    this->minePositions = nullptr;

    field = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        field[i] = new int[columns] {0};
    }

    setMines();
}

Minefield::~Minefield() {
    for(int i = 0; i < rows; ++i) {
        delete[] field[i];
    }
    delete[] field;
    delete[] minePositions;
}

void Minefield::setMines() {
    srand(time(NULL));
    int minesSet = 0;
    const int total_cells = rows * columns;
    minePositions = new std::pair<int, int>[numberOfMines];

    while (minesSet < numberOfMines) {
        int ind = rand() % total_cells;
        int row = ind / columns;
        int col = ind % columns;

        if (field[row][col] != 9) {
            field[row][col] = 9;
            minePositions[minesSet] = std::make_pair(row, col);
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
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
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

int Minefield::getRows() {
    return rows;
}

int Minefield::getColumns() {
    return columns;
}

std::pair<int, int>* Minefield::getMinePositions() const {
    return minePositions;
}

void Minefield::resetGame(int rows, int cols, int numberOfMines) {
    clearField();

    this->rows = rows;
    this->columns = cols;
    this->numberOfMines = numberOfMines;

    allocateField();
    setMines();
    std::cout<<std::endl; // TODO: Remove later
    show();
}

void Minefield::clearField() {
    if (field != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] field[i];
        }
        delete[] field;
    }
    if (minePositions != nullptr) {
        delete[] minePositions;
    }
}

void Minefield::allocateField() {
    field = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        field[i] = new int[columns] {0};
    }
    minePositions = new std::pair<int, int>[numberOfMines];
}
