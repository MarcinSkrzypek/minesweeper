#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <cstdlib>
#include <ctime>
#include <utility>

class Minefield
{
    public:
        Minefield();
        virtual ~Minefield();

        void setMines();
        void resetGame(int rows, int cols, int numberOfMines);
        void clearField();
        void show();
        void updateCellsAround(int row, int col);
        bool isValidCell(int row, int col);
        int check(int row, int column);
        int getNumberOfMines();
        std::pair<int,int>* getMinePositions() const;
        int getRows();
        int getColumns();

    private:
        int rows;
        int columns;
        int** field;
        int numberOfMines;
        std::pair<int,int>* minePositions;
};

#endif // MINEFIELD_H
