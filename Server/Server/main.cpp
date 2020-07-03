#include "Engine\hwindows.h"  // first
#include "Engine\exception.h"
#include "Engine\defines.h"
#include "game_server.h"

int WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
    try
    {
        return GameServer(9999).run();
    }
    catch (const Exception& e)
    {
#ifdef _DEBUG
        e.logError();
#else
		e.printError();
#endif
        Sleep(5000);
    }

    return -1;
}