#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Windows.h>
#include "Resource.h"

class GameMenu {
public:
    GameMenu();
    void initialize(HWND parentHwnd, HINSTANCE hInst);
    static void commandHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND parentHwnd;
    HINSTANCE hInst;
    HMENU hMenu;

    void setupMenu();
};

#endif // GAMEMENU_H
