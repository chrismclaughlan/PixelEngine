#include "hwindows.h"
#include "app.h"
#include "exception.h"

FILE* fConsole;

int WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(&fConsole, "CON", "w", stdout);

    try
    {
        return App().Run();
    }
    catch (AppException& e)
    {
        e.logError();
        Sleep(5000);
    }
}