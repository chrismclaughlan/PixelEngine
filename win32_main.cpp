#include <windows.h>
#include "win32_windows.h"
#include "types.h"

#include "demo.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	DemoWindow demoWindow(
		L"Demo Window", NO_WINDOW_RESIZING, 40, false, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 800
		);

	ShowWindow(demoWindow.Window(), nCmdShow);

	while (demoWindow.isRunning())
	{
		// Get user input
		MSG msg = {};
		demoWindow.resetButtons();
		while (PeekMessage(&msg, demoWindow.Window(), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		demoWindow.run();
		demoWindow.render();
		demoWindow.updateFPS();

	}  // while is_running

	return 0;
}