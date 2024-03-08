#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Windows.h>
#include "Minefield.h"
#include "MinefieldView.h"
#include "Resource.h"

class GameMenu {
public:
    GameMenu(Minefield& minefield);
    void initialize(HWND parentHwnd, HINSTANCE hInst, MinefieldView* minefieldView);
    void commandHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    Minefield& minefield;
    MinefieldView* minefieldView;
    HWND parentHwnd;
    HINSTANCE hInst;
    HMENU hMenu;

    void setupMenu();
};

INT_PTR CALLBACK DifficultyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif // GAMEMENU_H
