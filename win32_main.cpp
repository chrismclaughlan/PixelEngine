// Example of how to implement

#include <windows.h>
#include "win32_windows.h"
#include "types.h"
#include "input.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	MainWindow main_window;

	if (!main_window.Create(L"Window Name", NO_WINDOW_RESIZING))
	{
		return 0;
	}

	ShowWindow(main_window.Window(), nCmdShow);

	main_window.setHDC();
	main_window.initFPS();

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
		// --> implement something here <--

		// Render
		main_window.render();

		main_window.updateFPS();

	}  // while is_running

	return 0;
}