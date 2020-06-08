#include <windows.h>
#include "win32_windows.h"
#include "types.h"

#include "Instance.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

const wchar_t WINDOW_NAME[] = L"Primary Window";

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	win32::PrimaryWindow<Instance> primary_window(WINDOW_NAME, L"Primary Window class");

	if (!primary_window.Create(WINDOW_NAME, NO_WINDOW_RESIZING))
	{
		return 0;
	}

	ShowWindow(primary_window.Window(), nCmdShow);

	while (primary_window.isRunning())
	{
		// Get user input
		MSG msg = {};
		primary_window.resetButtons();
		while (PeekMessage(&msg, primary_window.Window(), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		primary_window.simulate();
		primary_window.render();
		primary_window.updateFPS();

	}  // while is_running

	return 0;
}