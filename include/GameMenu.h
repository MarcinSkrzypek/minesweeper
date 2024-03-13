#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Windows.h>
#include "Minefield.h"
#include "MinefieldView.h"
#include "Resource.h"
#include "GameConfig.h"
#include "DifficultyLevel.h"
#include "BitmapLoader.h"
#include "FontLoader.h"
#include <tchar.h>

class GameMenu {
  public:
    GameMenu(Minefield& minefield, BitmapLoader& bitmapLoader);
    void initialize(HWND parentHwnd, HINSTANCE hInst, MinefieldView* minefieldView);
    void commandHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void updateWindowTitle(Minefield& minefield);
    void updateMineCounter(int currentMinesCount);

  private:
    Minefield& minefield;
    BitmapLoader& bitmapLoader;
    MinefieldView* minefieldView;
    FontLoader fontLoader;
    HWND parentHwnd;
    HINSTANCE hInst;
    HMENU hMenu;
    HWND hMineImage;
    HWND hMineCounter;

    void setupMenu();
    void setupMineCounter(HWND parentHwnd, HINSTANCE hInst, BitmapLoader& bitmapLoader, MinefieldView* minefieldView);
};

INT_PTR CALLBACK DifficultyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif // GAMEMENU_H
