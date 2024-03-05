#include "Window.h"
#include <stdlib.h>

int main()
{
    Minefield minefield;
    BitmapLoader bitmapLoader;
    Window app(minefield, bitmapLoader);
    if (app.init())
    {
        while (app.isRun())
        {
            app.broadcast();
        }
    }
    return 0;
}
