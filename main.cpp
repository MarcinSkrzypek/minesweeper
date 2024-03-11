#include "Window.h"
#include <stdlib.h>

int main() {
    Minefield minefield;
    BitmapLoader bitmapLoader;
    GameMenu gameMenu(minefield);
    Window app(minefield, bitmapLoader, gameMenu);
    if (app.init()) {
        while (app.isRun()) {
            app.broadcast();
        }
    }
    return 0;
}
