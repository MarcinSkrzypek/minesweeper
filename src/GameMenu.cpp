#include "GameMenu.h"

GameMenu::GameMenu(Minefield& minefield) : minefield(minefield), minefieldView(nullptr), parentHwnd(nullptr), hInst(nullptr), hMenu(nullptr) {}

void GameMenu::initialize(HWND parentHwnd, HINSTANCE hInst, MinefieldView* minefieldView) {
    this->parentHwnd = parentHwnd;
    this->hInst = hInst;
    this->minefieldView = minefieldView;
    setupMenu();
}

void GameMenu::setupMenu() {
    hMenu = CreateMenu();

    HMENU hSubMenu = CreatePopupMenu();
    AppendMenuW(hSubMenu, MF_STRING, IDM_NEW_GAME, L"New game");
    AppendMenuW(hSubMenu, MF_STRING, IDM_DIFFICULTY, L"Difficulty");
    AppendMenuW(hSubMenu, MF_SEPARATOR, IDM_SEPARATOR, NULL);
    AppendMenuW(hSubMenu, MF_STRING, IDM_EXIT, L"Exit");

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Options");

    SetMenu(parentHwnd, hMenu);
}

void GameMenu::commandHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(LOWORD(wParam)) {
        case IDM_NEW_GAME:
            minefield.resetGame();
            minefieldView->resetCells();
            break;
        case IDM_DIFFICULTY:
            MessageBoxW(hwnd, L"Difficulty settings", L"Info", MB_OK | MB_ICONINFORMATION);
            break;
        case IDM_EXIT:
            MessageBoxW(hwnd, L"Exit", L"Info", MB_OK | MB_ICONINFORMATION);
            break;
        default:
            break;
    }
}
