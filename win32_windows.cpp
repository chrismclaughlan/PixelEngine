#include <windows.h>
#include <windowsX.h>  // mouse movement
#include "win32_windows.h"
#include "types.h"
#include "input.h"

#define HIDE_CURSOR true

namespace win32
{
LRESULT MainWindow::HandleMessage
(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (HIDE_CURSOR) ShowCursor(false);
		return 0;
	} break;

	case WM_DESTROY:
	{
		handleDestory();
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		if (shouldClose())
			DestroyWindow(m_hwnd);
		else
			return 0;
	} break;

	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		renderer.sizeChangeWin32(&rect);
	} break;

	case WM_KEYUP:  // nessessary
	case WM_KEYDOWN:
	{
		handleKeyDown(wParam, lParam);
	} break;
	case WM_MOUSEMOVE:
	{
		handleMouseMove(wParam, lParam);
	} break;
	case WM_LBUTTONUP:  // nessessary
	{
		handleMouseLeftButtonUp(wParam, lParam);
	} break;
	case WM_LBUTTONDOWN:
	{
		handleMouseLeftButtonDown(wParam, lParam);
	} break;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::handleDestory()
{
	is_running = false;
}

bool MainWindow::shouldClose()
{
	bool should_close = false;

	if (HIDE_CURSOR) ShowCursor(true);
	if (MessageBox(m_hwnd, L"Really quit?", window_name, MB_OKCANCEL) == IDOK)
		should_close = true;

	if (HIDE_CURSOR) ShowCursor(false);
	return should_close;
}

void MainWindow::handleKeyDown(WPARAM wParam, LPARAM lParam)
{
	uint32 vk_code = wParam;
	bool new_is_down = ((lParam & (1 << 31)) == 0);

	switch (wParam)
	{
#include "input.cpp"
	default:
		break;
	}
}

void MainWindow::handleMouseMove(WPARAM wParam, LPARAM lParam)
{
	input.mouse_x_pos = GET_X_LPARAM(lParam);
	input.mouse_y_pos = -(GET_Y_LPARAM(lParam) - renderer.getHeight());
}

void MainWindow::handleMouseLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	input.mouse_click = false;
}

void MainWindow::handleMouseLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	input.mouse_click = true;
}

void MainWindow::setHDC()
{
	if (!hdc) hdc = GetDC(m_hwnd);
}

void MainWindow::initFPS()
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

// Business methods //

void MainWindow::init()
{
	setHDC();
	initFPS();
}

void MainWindow::render()
{
	render::RenderState* render_state = renderer.getRenderState();
	StretchDIBits(hdc, 0, 0, render_state->width, render_state->height, 0, 0,
		render_state->width, render_state->height, render_state->memory,
		&render_state->bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}

void MainWindow::updateFPS()
{
	QueryPerformanceCounter(&performance.end_time);
	performance.delta_time = ((float)(performance.end_time.QuadPart -
		performance.begin_time.QuadPart)) / performance.frequency;
	performance.begin_time = performance.end_time;
}

bool MainWindow::isRunning()
{
	return (hdc ? is_running : FALSE);
}

void MainWindow::resetButtons()
{
	for (int32 i = 0; i < input::BUTTON_COUNT; i++)
	{
		input.buttons[i].has_changed = false;
	}
}
}  // namespace win32