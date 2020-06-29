#include "hwindows.h"
#include "app.h"

int WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	// TODO catch exceptions
	return App().Run();
}