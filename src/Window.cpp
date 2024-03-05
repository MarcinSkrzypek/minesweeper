#include "Window.h"
#include "Cell.h"
#include "BitmapLoader.h"

BitmapLoader bitmapLoader;

Window::Window() : m_is_run(false), cells(nullptr), rows(10), cols(10) {
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
                        int id = 100 + (i + j * window->cols);
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


bool Window::init()
{
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

bool Window::broadcast()
{
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

bool Window::release()
{
    if (!DestroyWindow(m_hwnd))
    {
        return false;
    }
    return true;
}

bool Window::isRun()
{
    return m_is_run;
}

void Window::onCreate()
{
    bitmapLoader.loadImages();
    createCells();
}

void Window::onUpdate()
{

}

void Window::onCommand(int wmId) {
    // TODO:
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j] && cells[i][j]->getId() == wmId) {
                HBITMAP oneBitmap = bitmapLoader.getImage(L"One");
                SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)oneBitmap);
                break;
            }
        }
    }
}

void Window::onRightClick(int wmId) {
    // TODO:
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j] && cells[i][j]->getId() == wmId) {
                HBITMAP qmBitmap = bitmapLoader.getImage(L"QuestionMark");
                SendMessage(cells[i][j]->getHandle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)qmBitmap);
                break;
            }
        }
    }
}

void Window::onDestroy()
{
    m_is_run = false;
}

void Window::createCells() {
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            int id = 100 + (i + j * cols);
            cells[i][j] = new Cell(m_hwnd, GetModuleHandle(nullptr), 8 + 32 * i, 56 + 32 * j, 32, 32, id);
        }
    }
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
