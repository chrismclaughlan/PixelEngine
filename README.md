# Base Windows
 
### How to use:
Input -> Create enum for button in input.h and call process_button() for corresponding VK code in input.cpp

Main -> Create PrimaryWindow object in win32_main, call Create() and then ShowWindow(). Then it is ready to 
be placed in loop. eg.
```
PrimaryWindow<Game> primary_window(WINDOW_NAME, L"Primary Window class");

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
```