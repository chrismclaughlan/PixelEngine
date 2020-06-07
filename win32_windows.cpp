#include <windows.h>
#include "win32_basewindow.h"
#include "win32_windows.h"
#include "types.h"

LRESULT MainWindow<class T>::HandleMessage
(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		is_running = false;
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		if (MessageBox(m_hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(m_hwnd);
		}
		else
			return 0;
	} break;

	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		renderer.sizeChangeWin32(&rect);
	} break;

	//case WM_KEYUP:
	case WM_KEYDOWN: handleKeyDown(wParam, lParam); break;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow<class T>::handleKeyDown(WPARAM wParam, LPARAM lParam)
{
	uint32 vk_code = (uint32)wParam;
	bool is_down = ((lParam & (1 << 31)) == 0);

	switch (vk_code)
	{
		process_button(BUTTON_UP, VK_UP);
		process_button(BUTTON_DOWN, VK_DOWN);
		process_button(BUTTON_LEFT, VK_LEFT);
		process_button(BUTTON_RIGHT, VK_RIGHT);
		process_button(BUTTON_SPACE, VK_SPACE);
		process_button(BUTTON_RETURN, VK_RETURN);
		process_button(BUTTON_ESCAPE, VK_ESCAPE);
	}
}

void MainWindow<class T>::resetButtons()
{
	for (int32 i = 0; i < BUTTON_COUNT; i++)
	{
		input.buttons[i].has_changed = false;
	}
}

bool MainWindow<class T>::isRunning()
{
	return (hdc ? is_running : FALSE);
}
void MainWindow<class T>::setHDC()
{
	if (!hdc) hdc = GetDC(m_hwnd);
}

// Business methods //

void MainWindow<class T>::initFPS()
{
	performance.delta_time = 0.016666f;
	QueryPerformanceCounter(&performance.begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}
}

void MainWindow<class T>::updateFPS()
{
	QueryPerformanceCounter(&performance.end_time);
	performance.delta_time = ((float)(performance.end_time.QuadPart -
		performance.begin_time.QuadPart)) / performance.frequency;
	performance.begin_time = performance.end_time;
}

void MainWindow<class T>::render()
{
	RenderState* render_state = renderer.getRenderState();
	StretchDIBits(hdc, 0, 0, render_state->width, render_state->height, 0, 0,
	render_state->width, render_state->height, render_state->memory,
	&render_state->bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}