#ifndef WINDOW_H
#define WINDOW_H

#include <Window.h>
#include "Cell.h"
#include <vector>

class Window
{
public:
    Window();
    ~Window();

    bool init();
    bool broadcast();
    bool release();
    bool isRun();

    // EVENTS
    void onCreate();
    void onUpdate();
    void onDestroy();
    void onCommand(int wmId); // Handle left-click actions
    void onRightClick(int wmId); // Handle right-click actions

    void createCells();
    void cellReveal(int i, int j);
    void cascadeReveal(int i, int j);
    void updateCellOnRightClick(Cell* cell);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    HWND m_hwnd;
    bool m_is_run;
    Cell ***cells;
    int rows, cols;
};

#endif // WINDOW_H
