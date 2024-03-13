#include "Timer.h"

Timer::Timer(FontLoader& fontLoader, HWND parentHwnd, HINSTANCE hInst, UINT_PTR timerID, UINT elapse)
    : timerID(timerID), fontLoader(fontLoader), parentHwnd(parentHwnd), hInst(hInst), elapse(elapse), running(false) {}

Timer::~Timer() {
    stop();
}

void Timer::start() {
    startTime = GetTickCount();

    RECT rcClient;
    GetClientRect(parentHwnd, &rcClient);
    int windowWidth = rcClient.right - rcClient.left;
    int timerX = windowWidth - 90;

    HFONT hDigitalFont = fontLoader.getFont(L"Digital-7 Mono");
    hTimer = CreateWindowExW(0, L"STATIC", L"00:00",
                                 WS_VISIBLE | WS_CHILD,
                                 timerX, 10, 100, 32,
                                 parentHwnd, NULL, hInst, NULL);
    SendMessage(hTimer, WM_SETFONT, (WPARAM)hDigitalFont, TRUE);

    if (!running) {
        SetTimer(parentHwnd, timerID, elapse, NULL);
        running = true;
    }
}

void Timer::stop() {
    if (running) {
        KillTimer(parentHwnd, timerID);
        running = false;
    }
}

void Timer::update() {
    DWORD currentTime = GetTickCount();
    DWORD elapsedTime = (currentTime - startTime) / 1000;

    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    std::wstringstream wss;
    wss << std::setw(2) << std::setfill(L'0') << minutes << L":" << std::setw(2) << std::setfill(L'0') << seconds;

    SetWindowTextW(hTimer, wss.str().c_str());
}

void Timer::reset() {
    if (running) {
        KillTimer(parentHwnd, timerID);
    }
    SetTimer(parentHwnd, timerID, elapse, NULL);
    startTime = GetTickCount();
    running = true;
    SetWindowTextW(hTimer, L"00:00");
}
