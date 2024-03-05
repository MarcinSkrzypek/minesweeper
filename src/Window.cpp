#include "Window.h"
#include "Cell.h"
#include "BitmapLoader.h"
#include "Minefield.h"

BitmapLoader bitmapLoader;
Minefield minefield;

Window::Window() : m_is_run(false), cells(nullptr), rows(minefield.getRows()), cols(minefield.getColumns()) {
    cells = new Cell**[rows];
    for (int i = 0; i < rows; ++i) {
        cells[i] = new Cell*[cols];
        for (int j = 0; j < cols; ++j) {
            cells[i][j] = nullptr;
        }
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg){
        case WM_CREATE: {
            LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
            window = static_cast<Window*>(pcs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
            window->m_hwnd = hwnd;
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            window->onDestroy();
            PostQuitMessage(0);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wparam);
            window->onCommand(wmId);
            break;
        }
        case WM_CONTEXTMENU:{
            HWND hwndControl = (HWND)wparam;
            for (int i = 0; i < window->rows; ++i) {
                for (int j = 0; j < window->cols; ++j) {
                    if (window->cells[i][j] && window->cells[i][j]->getHandle() == hwndControl) {
                        int id = 100 + (i * window->cols + j);
                        window->onRightClick(id);
                        break;
                    }
                }
            }
            break;
        }
        default: {
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
    return NULL;
}


bool Window::init() {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "MyWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    m_hwnd = CreateWindowEx(
                 WS_EX_OVERLAPPEDWINDOW,
                 "MyWindowClass",
                 "Minesweeper",
                 WS_OVERLAPPEDWINDOW,
                 CW_USEDEFAULT,
                 CW_USEDEFAULT,
                 32*cols+32,
                 32*rows+106,
                 NULL, NULL, GetModuleHandle(nullptr), this);

    SetLayeredWindowAttributes(m_hwnd, NULL, 255, LWA_ALPHA);

    if (!m_hwnd)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    onCreate();

    // Flag indicating that the window is initialized and running
    m_is_run = true;

    return true;
}

bool Window::broadcast() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    onUpdate();
    Sleep(1);
    return true;
}

bool Window::release() {
    if (!DestroyWindow(m_hwnd))
    {
        return false;
    }
    return true;
}

bool Window::isRun() {
    return m_is_run;
}

void Window::onCreate() {
    bitmapLoader.loadImages();
    createCells();
    minefield.show();
}

void Window::onUpdate() {

}

void Window::onCommand(int wmId) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j] && cells[i][j]->getId() == wmId) {
                cellReveal(i, j);
                return;
            }
        }
    }
}

void Window::onRightClick(int wmId)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (cells[i][j] && cells[i][j]->getId() == wmId)
            {
                updateCellOnRightClick(cells[i][j]);
                return;
            }
        }
    }
}

void Window::onDestroy() {
    m_is_run = false;
}

Window::~Window() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete cells[i][j];
        }
        delete[] cells[i];
    }
    delete[] cells;
}

void Window::createCells() {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int id = 100 + (i * cols + j);
            cells[i][j] = new Cell(m_hwnd, GetModuleHandle(nullptr), 8 + 32 * j, 56 + 32 * i, 32, 32, id);
        }
    }
}

void Window::cellReveal(int i, int j)
{
    int fieldValue = minefield.check(i, j);

    if (fieldValue == 0)
    {
        cascadeReveal(i, j);
    }
    else
    {
        HBITMAP bitmap = bitmapLoader.getBitmapForValue(fieldValue);
        if (bitmap != nullptr)
        {
            SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
            cells[i][j]->setState(CellState::Revealed);
        }
    }
}

void Window::cascadeReveal(int i, int j) {
    if (cells[i][j]->getState() == CellState::Revealed) {
        return;
    }

    HBITMAP bitmap = bitmapLoader.getBitmapForValue(minefield.check(i, j));
    if (bitmap != nullptr) {
        SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
        cells[i][j]->setState(CellState::Revealed);
    }

    if (minefield.check(i, j) != 0) {
        return;
    }

    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int newI = i + di;
            int newJ = j + dj;

            if (di == 0 && dj == 0) continue;

            if (minefield.isValidCell(newI, newJ)) {
                if (cells[newI][newJ]->getState() != CellState::Revealed) {
                    cascadeReveal(newI, newJ);
                }
            }
        }
    }
}

void Window::updateCellOnRightClick(Cell* cell)
{
    HBITMAP bitmap = nullptr;
    switch(cell->getState())
    {
    case CellState::Unrevealed:
        bitmap = bitmapLoader.getImage(L"MinesGuess");
        cell->setState(CellState::Guessed);
        break;
    case CellState::Revealed:
        return;
    case CellState::Questioned:
        cell->setState(CellState::Unrevealed);
        break;
    case CellState::Guessed:
        bitmap = bitmapLoader.getImage(L"QuestionMark");
        cell->setState(CellState::Questioned);
        break;
    }

    if (bitmap != nullptr)
    {
        SendMessage(cell->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
    }
    else
    {
        SendMessage(cell->getHandle(), BM_SETIMAGE, IMAGE_BITMAP, NULL);
    }
}
