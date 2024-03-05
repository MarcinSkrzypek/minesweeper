#ifndef WINDOW_H
#define WINDOW_H

#include <Window.h>
#include "Cell.h"
#include "Minefield.h"
#include "MinefieldView.h"

class Window
{
public:
    Window(Minefield& minefield, BitmapLoader& bitmapLoader);
    ~Window();

    bool init();
    bool broadcast();
    bool release();
    bool isRun();

    // EVENTS
    void onCreate();
    void onUpdate();
    void onDestroy();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    bool m_is_run;
    Minefield& minefield;
    HWND m_hwnd;
    HINSTANCE m_hInst;
    BitmapLoader& bitmapLoader;
    MinefieldView* minefieldView;
};

#endif // WINDOW_H
