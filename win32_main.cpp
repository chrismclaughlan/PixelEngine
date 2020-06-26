#include <windows.h>
#include "win32_windows.h"
#include "types.h"

#include "game.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

PCWSTR WINDOW_NAME = L"Game Window";

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	GameWindow gameWindow(
		WINDOW_NAME, (DWORD)WS_OVERLAPPEDWINDOW, false, 0, 
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800);

	ShowWindow(gameWindow.Window(), nCmdShow);

	while (gameWindow.isRunning())
	{
		// Get user input
		MSG msg = {};
		gameWindow.resetButtons();
		while (PeekMessage(&msg, gameWindow.Window(), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		gameWindow.run();
		gameWindow.render();
		gameWindow.updateFPS();

	}  // while is_running

	return 0;
}