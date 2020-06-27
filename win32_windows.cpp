#include "win32_windows.h"
#include <windowsX.h>  // mouse movement
#include "win32_mainwindow.h"
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
	case WM_RBUTTONUP:
	{
		handleMouseRightButtonUp(wParam, lParam);
	} break;
	case WM_RBUTTONDOWN:
	{
		handleMouseRightButtonDown(wParam, lParam);
	} break;
	case WM_MBUTTONUP:
	{
		handleMiddleMouseButtonUp(wParam, lParam);
	} break;
	case WM_MBUTTONDOWN:
	{
		handleMiddleMouseButtonDown(wParam, lParam);
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
	input.left_click = false;
}

void MainWindow::handleMouseLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCapture(m_hwnd);
	input.left_click = true;
}

void MainWindow::handleMouseRightButtonUp(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	input.right_click = false;
}

void MainWindow::handleMouseRightButtonDown(WPARAM wParam, LPARAM lParam)
{
	SetCapture(m_hwnd);
	input.right_click = true;
}

void MainWindow::handleMiddleMouseButtonUp(WPARAM, LPARAM)
{
	input.middle_click = false;
}

void MainWindow::handleMiddleMouseButtonDown(WPARAM, LPARAM)
{
	input.middle_click = true;
}

void MainWindow::setHDC()
{
	if (!hdc) hdc = GetDC(m_hwnd);
}

BOOL MainWindow::setWindowPos(int32 x, int32 y)
{
	if (m_hwnd != NULL)
	{
		RECT rect;
		if (!GetWindowRect(m_hwnd, &rect))
		{
			return FALSE;
		}
		if (!MoveWindow(m_hwnd, x, y, rect.bottom - rect.top, rect.right - rect.left, TRUE))
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MainWindow::setWindowSize(int32 x, int32 y)
{
	if (m_hwnd != NULL)
	{
		RECT rect;
		if (!GetWindowRect(m_hwnd, &rect))
		{
			return FALSE;
		}
		if (!MoveWindow(m_hwnd, rect.left, rect.top, x, y, TRUE))
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

// Business methods //

void MainWindow::render()
{
	render::RenderState* render_state = rend.getRenderState();
	StretchDIBits(hdc, 0, 0, render_state->width, render_state->height, 0, 0,
		render_state->width, render_state->height, render_state->memory,
		&render_state->bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
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