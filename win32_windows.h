#pragma once
#include <windows.h>
#include "win32_basewindow.h"
#include "render.h"
#include "input.h"

namespace win32
{
struct Performance
{
	float frequency;
	float delta_time;
	LARGE_INTEGER begin_time;
	LARGE_INTEGER end_time;
};

class MainWindow :public BaseWindow<MainWindow>
{
protected:
	MainWindow() = delete;
	MainWindow(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		bool hideCursor = false,  // added param
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		) : BaseWindow(), hide_cursor(hideCursor)
	{
		Create(lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, hMenu);
		initFPS();
		setHDC();
	}
protected:
	PCWSTR window_name = L"MainWindow";  // default name -> change
	PCWSTR class_name = L"MainWindow class";
	HDC hdc;
	input::Input input = {};
	render::Render renderer = {};
	Performance performance = {};
	bool is_running = true;
	bool hide_cursor = false;

	void initFPS();
	void setHDC();

protected:
	void handleDestory();
	bool shouldClose();  // returns true if window should close
	void handleKeyDown(WPARAM, LPARAM);
	void handleKeyUp(WPARAM, LPARAM) {}
	void handleMouseMove(WPARAM, LPARAM);
	void handleMouseLeftButtonUp(WPARAM, LPARAM);
	void handleMouseLeftButtonDown(WPARAM, LPARAM);

protected:

public:
	LRESULT HandleMessage(UINT, WPARAM, LPARAM);
	PCWSTR ClassName() const { return class_name; }
	void setClassName(PCWSTR name) { class_name = name; }

	// Business methods //
	void render();
	void updateFPS();
	bool isRunning();
	void resetButtons();
};
}  // namespace win32