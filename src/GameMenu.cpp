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
        GameConfig::setGameOverFlag(false);
        break;
    case IDM_DIFFICULTY: {
        std::pair<Minefield*, MinefieldView*> params = {&minefield, minefieldView};
        DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIFFICULTY_DIALOG), parentHwnd, DifficultyDialogProc, reinterpret_cast<LPARAM>(&params));
        SetWindowPos(hwnd, NULL, 0, 0, 32 * minefield.getColumns() + 32, 32 * minefield.getRows() + 106, SWP_NOMOVE | SWP_NOZORDER);
        break;
    }
    case IDM_EXIT:
        DestroyWindow(hwnd);
        break;
    default:
        break;
    }
}

// Helper function for DifficultyDialogProc CALLBACK
void SetDialogControls(HWND hDlg, DifficultyLevel difficulty, Minefield* minefield, BOOL isInit) {
    int radioButtonID = IDC_RADIO_CUSTOM;
    switch(difficulty) {
    case DifficultyLevel::Beginner:
        radioButtonID = IDC_RADIO_BEGINNER;
        break;
    case DifficultyLevel::Intermediate:
        radioButtonID = IDC_RADIO_INTERMEDIATE;
        break;
    case DifficultyLevel::Expert:
        radioButtonID = IDC_RADIO_EXPERT;
        break;
    case DifficultyLevel::Custom:
        radioButtonID = IDC_RADIO_CUSTOM;
        break;
    }

    CheckRadioButton(hDlg, IDC_RADIO_BEGINNER, IDC_RADIO_CUSTOM, radioButtonID);
    BOOL enableFields = (difficulty == DifficultyLevel::Custom);

    EnableWindow(GetDlgItem(hDlg, IDC_EDIT_ROWS), enableFields);
    EnableWindow(GetDlgItem(hDlg, IDC_EDIT_COLS), enableFields);
    EnableWindow(GetDlgItem(hDlg, IDC_EDIT_MINES), enableFields);

    SendMessage(GetDlgItem(hDlg, IDC_EDIT_ROWS), EM_SETREADONLY, !enableFields, 0);
    SendMessage(GetDlgItem(hDlg, IDC_EDIT_COLS), EM_SETREADONLY, !enableFields, 0);
    SendMessage(GetDlgItem(hDlg, IDC_EDIT_MINES), EM_SETREADONLY, !enableFields, 0);

    if (!isInit && difficulty != DifficultyLevel::Custom) {
        auto settings = GameConfig::getDifficultySettings(difficulty);
        SetDlgItemInt(hDlg, IDC_EDIT_ROWS, settings.rows, FALSE);
        SetDlgItemInt(hDlg, IDC_EDIT_COLS, settings.cols, FALSE);
        SetDlgItemInt(hDlg, IDC_EDIT_MINES, settings.mines, FALSE);
    } else {
        SetDlgItemInt(hDlg, IDC_EDIT_ROWS, minefield->getRows(), FALSE);
        SetDlgItemInt(hDlg, IDC_EDIT_COLS, minefield->getColumns(), FALSE);
        SetDlgItemInt(hDlg, IDC_EDIT_MINES, minefield->getNumberOfMines(), FALSE);
    }
}

// Helper function for DifficultyDialogProc CALLBACK
BOOL ValidateAndExtractInput(HWND hDlg, int& rows, int& cols, int& mines) {
    TCHAR strRows[5], strCols[5], strMines[5];
    GetDlgItemText(hDlg, IDC_EDIT_ROWS, strRows, 5);
    GetDlgItemText(hDlg, IDC_EDIT_COLS, strCols, 5);
    GetDlgItemText(hDlg, IDC_EDIT_MINES, strMines, 5);

    rows = _tstoi(strRows);
    cols = _tstoi(strCols);
    mines = _tstoi(strMines);

    if (rows < 1 || rows > GameConfig::MAX_ROWS) {
        MessageBoxW(hDlg, L"The number of rows is out of range.", L"Error", MB_ICONERROR);
        return FALSE;
    }
    if (cols < 1 || cols > GameConfig::MAX_COLUMNS) {
        MessageBoxW(hDlg, L"The number of columns is out of range.", L"Error", MB_ICONERROR);
        return FALSE;
    }
    if (mines < 1 || mines > rows * cols - 1) {
        MessageBoxW(hDlg, L"The number of mines is out of range.", L"Error", MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

// Helper function for DifficultyDialogProc CALLBACK
void HandleDifficultySelection(HWND hDlg, WPARAM wParam, Minefield* minefield, MinefieldView* minefieldView) {
    if(LOWORD(wParam) >= IDC_RADIO_BEGINNER && LOWORD(wParam) <= IDC_RADIO_CUSTOM) {
        DifficultyLevel newDifficulty;

        if(LOWORD(wParam) == IDC_RADIO_BEGINNER) {
            newDifficulty = DifficultyLevel::Beginner;
        } else if(LOWORD(wParam) == IDC_RADIO_INTERMEDIATE) {
            newDifficulty = DifficultyLevel::Intermediate;
        } else if(LOWORD(wParam) == IDC_RADIO_EXPERT) {
            newDifficulty = DifficultyLevel::Expert;
        } else if(LOWORD(wParam) == IDC_RADIO_CUSTOM) {
            newDifficulty = DifficultyLevel::Custom;
        }

        SetDialogControls(hDlg, newDifficulty, minefield, FALSE);
    }
    if(LOWORD(wParam) == IDOK) {
        int rows, cols, mines;
        if(ValidateAndExtractInput(hDlg, rows, cols, mines)) {
            if (IsDlgButtonChecked(hDlg, IDC_RADIO_BEGINNER) == BST_CHECKED) {
                GameConfig::setCurrentDifficulty(DifficultyLevel::Beginner);
            } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_INTERMEDIATE) == BST_CHECKED) {
                GameConfig::setCurrentDifficulty(DifficultyLevel::Intermediate);
            } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_EXPERT) == BST_CHECKED) {
                GameConfig::setCurrentDifficulty(DifficultyLevel::Expert);
            } else if (IsDlgButtonChecked(hDlg, IDC_RADIO_CUSTOM) == BST_CHECKED) {
                GameConfig::setCurrentDifficulty(DifficultyLevel::Custom);
            }

            minefield->resetGame(rows, cols, mines);
            minefieldView->resetCells(rows, cols);
            GameConfig::setGameOverFlag(false);
            EndDialog(hDlg, IDOK);
        }
    } else if(LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, IDCANCEL);
    }
}

INT_PTR CALLBACK DifficultyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static Minefield* minefield = nullptr;
    static MinefieldView* minefieldView = nullptr;

    switch (message) {
    case WM_INITDIALOG: {
        auto params = reinterpret_cast<std::pair<Minefield*, MinefieldView*>*>(lParam);
        minefield = params->first;
        minefieldView = params->second;

        SetDialogControls(hDlg, GameConfig::getCurrentDifficulty(), minefield, TRUE);

        return (INT_PTR)TRUE;
    }
    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        HandleDifficultySelection(hDlg, wParam, minefield, minefieldView);
        return (INT_PTR)TRUE;
        break;
    }
    return (INT_PTR)FALSE;
}
