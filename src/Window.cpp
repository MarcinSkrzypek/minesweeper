#include "Window.h"

Window::Window(Minefield& minefield, BitmapLoader& bitmapLoader, GameMenu& gameMenu)
    : m_is_run(false), minefield(minefield), m_hwnd(NULL), m_hInst(GetModuleHandle(nullptr)),
      bitmapLoader(bitmapLoader), minefieldView(nullptr), gameMenu(gameMenu) {
}

Window::~Window() {
    if (minefieldView) {
        delete minefieldView;
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg) {
    case WM_CREATE: {
        LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
        window = static_cast<Window*>(pcs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY: {
        window->onDestroy();
        PostQuitMessage(0);
        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wparam);
        switch (wmId) {
        case IDM_NEW_GAME:
            window->gameMenu.commandHandler(hwnd, msg, wparam, lparam);
            break;
        case IDM_DIFFICULTY:
            window->gameMenu.commandHandler(hwnd, msg, wparam, lparam);
            break;
        case IDM_EXIT:
            window->gameMenu.commandHandler(hwnd, msg, wparam, lparam);
            break;
        default:
            window->minefieldView->handleCellLeftClick(wmId);
            break;
        }
        break;
    }
    case WM_CONTEXTMENU: {
        HWND hwndControl = (HWND)wparam;
        window->minefieldView->handleCellRightClick(hwndControl);
        break;
    }
    default: {
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    }
    return 0;
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

    if (!RegisterClassEx(&wc)) {
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
                 32*minefield.getColumns()+32,
                 32*minefield.getRows()+106,
                 NULL, NULL, GetModuleHandle(nullptr), this);

    SetLayeredWindowAttributes(m_hwnd, 0, 255, LWA_ALPHA);

    if (!m_hwnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    minefieldView = new MinefieldView(minefield, m_hwnd, m_hInst, bitmapLoader);

    onCreate();

    // Flag indicating that the window is initialized and running
    m_is_run = true;

    return true;
}

bool Window::broadcast() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    onUpdate();
    Sleep(1);
    return true;
}

bool Window::release() {
    if (!DestroyWindow(m_hwnd)) {
        return false;
    }
    return true;
}

bool Window::isRun() {
    return m_is_run;
}

void Window::onCreate() {
    GameConfig::setCurrentDifficulty(DifficultyLevel::Beginner);
    gameMenu.initialize(m_hwnd, GetModuleHandle(nullptr), minefieldView);
    minefieldView->initialize();
    minefield.show(); // TODO: Remove later
}

void Window::onUpdate() {

}

void Window::onDestroy() {
    m_is_run = false;
}
