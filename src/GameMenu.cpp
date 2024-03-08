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
            minefield.resetGame(minefield.getRows(), minefield.getColumns(), minefield.getNumberOfMines());
            minefieldView->resetCells(minefield.getRows(), minefield.getColumns());
            break;
        case IDM_DIFFICULTY: {
            std::pair<Minefield*, MinefieldView*> params = {&minefield, minefieldView};
            DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIFFICULTY_DIALOG), parentHwnd, DifficultyDialogProc, reinterpret_cast<LPARAM>(&params));
            SetWindowPos(hwnd, NULL, 0, 0, 32 * minefield.getColumns() + 32, 32 * minefield.getRows() + 106, SWP_NOMOVE | SWP_NOZORDER);
            break;
        }
        case IDM_EXIT:
            MessageBoxW(hwnd, L"Exit", L"Info", MB_OK | MB_ICONINFORMATION);
            break;
        default:
            break;
    }
}

INT_PTR CALLBACK DifficultyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static Minefield* minefield = nullptr;
    static MinefieldView* minefieldView = nullptr;

    if (message == WM_INITDIALOG) {
        auto params = reinterpret_cast<std::pair<Minefield*, MinefieldView*>*>(lParam);
        minefield = params->first;
        minefieldView = params->second;
        return (INT_PTR)TRUE;
    }

    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK: {
                    int rows, cols, mines;
                    if (IsDlgButtonChecked(hDlg, IDC_RADIO_BEGINNER) == BST_CHECKED) {
                        rows = cols = 9; mines = 10;
                    } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_INTERMEDIATE) == BST_CHECKED) {
                        rows = cols = 16; mines = 40;
                    } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_EXPERT) == BST_CHECKED) {
                        rows = 16; cols = 30; mines = 99;
                    } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_CUSTOM) == BST_CHECKED) {
                        // TODO: Add custom difficulty
                    }

                    minefield->resetGame(rows, cols, mines);
                    minefieldView->resetCells(rows, cols);

                    EndDialog(hDlg, IDOK);
                    return (INT_PTR)TRUE;
                }
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
