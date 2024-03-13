#include "Window.h"
#include <stdlib.h>

int main() {
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
