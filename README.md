# Base Windows

## TODO
- change input system (allow for single clicks)    (IN PROGRESS)
- implement new particles + particle generator
- create terraria clone
	- create expandable map
	- create player
		- can interact with world x squares from player with mouse
 
### How to use:
Input -> Create enum for button in input.h and call process_button() for corresponding VK code in input.cpp

Main -> Create PrimaryWindow object in win32_main, and then call ShowWindow(). Then it is ready to 
be placed in loop. 

Example win32_main.cpp
```
#include "win32_windows.h"
#include "win32_mainwindow.h"
#include "types.h"

#include "demo.h"

#define NO_WINDOW_RESIZING (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

int32 WINAPI wWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int32 nCmdShow)
{
	const int32 gridSize = 100;
	const int32 fpsLimit = 0;
	bool hideCursor = false;
	DemoWindow demoWindow(L"Demo Window", NO_WINDOW_RESIZING, gridSize, fpsLimit, hideCursor);

	//demoWindow.setWindowPos(100, 100);
	demoWindow.setWindowSize(600, 600);

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

	}  // while is_running

	return 0;
}
```