#pragma once
#include <windows.h>
#include "win32_basewindow.h"
#include "render.h"
#include "input.h"

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
	PCWSTR window_name = L"MainWindow";  // default name -> change
	PCWSTR class_name = L"MainWindow class";
	HDC hdc;
	Input input;
	Render renderer;
	Performance performance;
	bool is_running = true;

	void initFPS();
	void setHDC();

	// HandleMessage methods //
	void handleDestory();
	bool shouldClose();  // returns true if window should close
	void handleKeyDown(WPARAM, LPARAM);
	void handleKeyUp(WPARAM, LPARAM) {}

public:
	LRESULT HandleMessage(UINT, WPARAM, LPARAM);
	PCWSTR ClassName() const { return class_name; }
	void setClassName(PCWSTR nclass_name) { class_name = nclass_name; }

	// Business methods //
	void init();
	void render();
	void updateFPS();
	bool isRunning();
	void resetButtons();
};

template <class T>
class PrimaryWindow : public MainWindow
{
private:
	T* business_class;

	void handleDestory()
	{
		MainWindow::handleDestory();
		delete business_class;
	}

public:
	PrimaryWindow(PCWSTR nwindow_name, PCWSTR nclass_name)
	{
		MainWindow::MainWindow();
		window_name = nwindow_name;
		class_name = nclass_name;
		business_class = nullptr;
	}


	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		)
	{
		BOOL result;
		result = MainWindow::Create(lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, hMenu);
		MainWindow::init();
		business_class = new T(&performance, &renderer, &input, &is_running);
		return result;
	}

	void simulate()
	{
		business_class->simulate();
	}
};