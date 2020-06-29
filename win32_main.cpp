#include "win32_windows.h"
#include "win32_mainwindow.h"
#include "types.h"

#include "demo.h"

#include "app.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	return App().Run();
	//const int32 screenSize = 800;
	//const int32 gridSize = screenSize / 2;
	//const int32 fpsLimit = 0;
	//bool hideCursor = false;
	//DemoWindow demoWindow(L"Demo Window", NO_WINDOW_RESIZING, gridSize, fpsLimit, hideCursor);

	////demoWindow.setWindowPos(100, 100);
	//demoWindow.setWindowSize(screenSize, screenSize);

	//ShowWindow(demoWindow.Window(), nCmdShow);

	//while (demoWindow.isRunning())
	//{
	//	// Get user input
	//	MSG msg = {};
	//	demoWindow.resetButtons();
	//	while (PeekMessage(&msg, demoWindow.Window(), NULL, NULL, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//	demoWindow.run();
	//	demoWindow.render();

	//}  // while is_running

	//return 0;
}