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
private:
	PCWSTR class_name = L"MainWindow class";
	HDC hdc;
	Input input;
	Render renderer;
	Performance performance;
	bool is_running = true;

public:
	LRESULT HandleMessage(UINT, WPARAM, LPARAM);
	PCWSTR ClassName() const { return class_name; }
	void setClassName(PCWSTR nclass_name) { class_name = nclass_name; }

	void handleKeyDown(WPARAM, LPARAM);
	void resetButtons();
	bool isRunning();
	void setHDC();

	// Business methods //

	void initFPS();
	void updateFPS();
	void simulate();
	void render();
};