#include "hwindows.h"
#include "app.h"
#include "exception.h"

#define DISPLAY_DEBUG_CONSOLE

#ifdef DISPLAY_DEBUG_CONSOLE
FILE* fConsole;
#endif

int WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
#ifdef DISPLAY_DEBUG_CONSOLE
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(&fConsole, "CON", "w", stdout);
#endif

    try
    {
        return App().Run();
    }
    catch (AppException& e)
    {
#ifdef DISPLAY_DEBUG_CONSOLE
        e.logError();
        Sleep(5000);
#endif
    }
}