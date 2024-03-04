#ifndef WINDOW_H
#define WINDOW_H

#include <Window.h>
#include "Cell.h"
#include <vector>

using namespace std;

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
    void onRightClick(POINTS pt); // Handle right-click actions


private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    HWND m_hwnd;
    bool m_is_run;
    Cell ***cells;
    int rows, cols;

    void createCells();
};

#endif // WINDOW_H
