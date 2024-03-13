#ifndef TIMER_H
#define TIMER_H

#include "FontLoader.h"
#include <Windows.h>
#include <string>
#include <iomanip>
#include <sstream>

class Timer {
  public:
    UINT_PTR timerID;

    Timer(FontLoader& fontLoader, HWND parentHwnd, HINSTANCE hInst, UINT_PTR timerId, UINT elapse);
    ~Timer();

    void start();
    void stop();
    void update();
    void reset();

  private:
    FontLoader& fontLoader;
    HWND parentHwnd;
    HINSTANCE hInst;
    UINT elapse;
    bool running;
    DWORD startTime;
    HWND hTimer;
};

#endif // TIMER_H
