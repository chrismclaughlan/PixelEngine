#include "Engine\hwindows.h"  // first
#include "Engine\exception.h"
#include "Engine\defines.h"

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

    std::cout << "Server test";
    Sleep(2000);

    return -1;
}