#include <windows.h>
#include <windowsX.h>  // mouse movement
#include "win32_windows.h"
#include "types.h"
#include "input.h"

namespace win32
{
LRESULT MainWindow::HandleMessage
(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (hide_cursor) ShowCursor(false);
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
		ShowCursor(true);
		if (shouldClose())
		{
			DestroyWindow(m_hwnd);
		}
		else
		{
			if (hide_cursor)
				ShowCursor(false);

			return 0;
		}

	} break;

	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		rend.sizeChangeWin32(&rect);
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
	case WM_LBUTTONUP:
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
	return true;  // temp

	if (MessageBox(m_hwnd, L"Really quit?", window_name, MB_OKCANCEL) == IDOK)
		return true;

	return false;
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
	input.mouse_y_pos = -(GET_Y_LPARAM(lParam) - rend.getHeight());
}

void MainWindow::handleMouseLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	input.mouse_click = false;
}

void MainWindow::handleMouseLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCapture(m_hwnd);
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

//BOOL MainWindow::create
//(PCWSTR lpWindowName, DWORD dwStyle, bool hideCursor, DWORD dwExStyle, 
//	int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
//{
//	hide_cursor = hideCursor;
//	BOOL result = Create(lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, hMenu);
//	// After Create()
//	initFPS();
//	setHDC();
//	return result;
//}

// Business methods //

void MainWindow::render()
{
	render::RenderState* render_state = rend.getRenderState();
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