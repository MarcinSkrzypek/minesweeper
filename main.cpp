#include "Window.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    Minefield minefield;
    BitmapLoader bitmapLoader;
    FontLoader fontLoader;
    GameMenu gameMenu(minefield, bitmapLoader, fontLoader);
    Window app(minefield, bitmapLoader, gameMenu, fontLoader);

    if (app.init()) {
        while (app.isRun()) {
            app.broadcast();
        }
    }
    return 0;
}
