#include <windows.h>
#include "win32_windows.h"
#include "types.h"
#include "input.h"

//const int WINDOW_WIDTH = 600;
//const int WINDOW_HEIGHT = 600;

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	MainWindow main_window;

	if (!main_window.Create(L"Window Name", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	main_window.setHDC();
	main_window.initFPS();

	ShowWindow(main_window.Window(), nCmdShow);

	while (main_window.isRunning())
	{
		// Get user input
		MSG msg = {};
		main_window.resetButtons();
		while (PeekMessage(&msg, main_window.Window(), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Simulate
		main_window.simulate();

		// Render
		main_window.render();

		main_window.updateFPS();

	}  // while is_running

	return 0;
}