#include "Engine\hwindows.h"  // first
#include "Engine\exception.h"
#include "Engine\defines.h"
#include "game_client.h"

#if DISPLAY_DEBUG_CONSOLE
FILE* fConsole;
#endif

int WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
#if DISPLAY_DEBUG_CONSOLE
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(&fConsole, "CON", "w", stdout);
#endif

    try
    {
        return GameClient(100).run();
    }
    catch (AppException& e)
    {
#if DISPLAY_DEBUG_CONSOLE
        e.logError();
        Sleep(5000);
#endif
    }

    return -1;
}