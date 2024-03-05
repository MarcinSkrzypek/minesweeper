#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <iostream>
#include <ctime>

class Minefield
{
    public:
        Minefield();
        virtual ~Minefield();

        void setMines();
        void show();
        void updateCellsAround(int row, int col);
        bool isValidCell(int row, int col);
        int check(int row, int column);
        int getNumberOfMines();
        int* getMines();
        int getRows();
        int getColumns();

    private:
        int rows;
        int columns;
        int** field;
        int numberOfMines;
        int* minesOnField;
};

#endif // MINEFIELD_H
