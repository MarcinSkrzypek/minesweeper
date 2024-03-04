#include "Window.h"
#include <stdlib.h>

int main()
{
    Window app;
    if (app.init())
    {
        while (app.isRun())
        {
            app.broadcast();
        }
    }
    return 0;
}
