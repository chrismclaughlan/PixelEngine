#include "win32_window.h"
#include <windowsX.h>  // mouse movement
#include "resource.h"
#include "Keyboard.h"
#include <assert.h>

#define WINDOW_STYLE (WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU)
#define GetXY()\
const int32 x = GET_X_LPARAM(lParam); \
const int32 y = height - GET_Y_LPARAM(lParam); \

Window::WindowClass Window::WindowClass::wClass;  // TODO what is this

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(NULL))
{
	HICON hicon = static_cast<HICON>(LoadImage(
		hInstance, MAKEINTRESOURCE(APPICON), IMAGE_ICON, 32, 32, NULL));
	HICON hiconsm = static_cast<HICON>(LoadImage(
		hInstance, MAKEINTRESOURCE(APPICON), IMAGE_ICON, 16, 16, NULL));
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetHInstance();
	wc.hIcon = hicon;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetClassName();
	wc.hIconSm = hiconsm;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wClassName, GetHInstance());
}

const wchar_t* Window::WindowClass::GetClassName() noexcept
{
	return wClassName;
}

HINSTANCE Window::WindowClass::GetHInstance() noexcept
{
	return wClass.hInstance;
}

/* --------------------------------------------------------- */

Window::Window(const wchar_t* name, int32 width, int32 height)
	: width(width), height(height)
{
	// Calculate window size
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	if (AdjustWindowRect(&rect, WINDOW_STYLE, FALSE) == NULL)
	{
		// throw ...
		OutputDebugString((LPCWSTR)GetLastError());
		assert(false);
	}

	hwnd = CreateWindow(
		WindowClass::GetClassName(), name,
		WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL, NULL,
		WindowClass::GetHInstance(), this
		);

	if (hwnd == NULL)
	{
		// throw ...
		OutputDebugString((LPCWSTR)GetLastError());
		assert(false);
	}

	HDC hdc = GetDC(hwnd);
	if (hdc == NULL)
	{
		// throw ...
		OutputDebugString((LPCWSTR)GetLastError());
		assert(false);
	}

	// Before ShowWindow()
	pGraphics = std::make_unique<Win32Graphics>(hwnd, hdc);  // (hwnd);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
}
Window::~Window()
{
	DestroyWindow(hwnd);
}

void Window::SetTitle(const std::string name)
{
	const std::wstring title = ToWString(name);
	SetWindowTextW(hwnd, title.c_str());
}

Win32Graphics& Window::Gfx()
{
	return *pGraphics;
}

// Returns true if quitting window
// Passes exitCode through pointer
bool Window::ProcessMessages(int32* exitCode) noexcept
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			exitCode = (int32*)msg.wParam;
			return true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return false;
}

LRESULT CALLBACK Window::HandleMsgSetup
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* pThis = NULL;

	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (Window*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->hwnd = hwnd;
	}
	else
	{
		pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (pThis)
	{
		return pThis->HandleMessage(msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT Window::HandleMessage
(UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
#define PREV_KEY_DOWN() (lParam & 0x40000000)

	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	} return 0;
	case WM_KILLFOCUS:
	{
		keyboard.ClearState();
	} break;
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		Gfx().ChangeSize(&rect);
	} break;
	/* -------------------- KEYBOARD -------------------- */
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		if (!PREV_KEY_DOWN() || keyboard.AutorepeatIsEnabled())
		{
			keyboard.OnKeyPressed(static_cast<uint8>(wParam));
		}
	} break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		keyboard.OnKeyReleased(static_cast<uint8>(wParam));
	} break;
	case WM_CHAR:
	{
		keyboard.OnChar(static_cast<uint8>(wParam));
	} break;
	/* -------------------- MOUSE -------------------- */
	case WM_MOUSEMOVE:
	{
		GetXY();
		if (x < 0 || y < 0 || x >= width || y >= height)  // Outside of window
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))  // Dragging
			{
				mouse.MouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mouse.MouseLeave();
			}
		}
		else  // Inside of window
		{
			mouse.MouseMove(x, y);
			if (!mouse.IsInWindow())  // If mouse wasn't in window before
			{
				SetCapture(hwnd);
				mouse.MouseEnter();
			}
		}
	} break;
	case WM_LBUTTONUP:
	{
		GetXY();
		mouse.LeftReleased(x, y);
	} break;
	case WM_LBUTTONDOWN:
	{
		GetXY();
		mouse.LeftPressed(x, y);
	} break;
	case WM_RBUTTONUP:
	{
		GetXY();
		mouse.RightReleased(x, y);
	} break;
	case WM_RBUTTONDOWN:
	{
		GetXY();
		mouse.RightPressed(x, y);
	} break;
	case WM_MBUTTONUP:
	{
		GetXY();
		mouse.MiddleReleased(x, y);
	} break;
	case WM_MBUTTONDOWN:
	{
		GetXY();
		mouse.MiddlePressed(x, y);
	} break;
	case WM_MOUSEWHEEL:
	{
		GetXY();
		const int32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.WheelDelta(x, y, delta);
	} break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}