#pragma once
#include "win32_windows.h"
#include "types.h"
#include "win32_graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <memory>
#include <string>

inline const std::wstring ToWString(const std::string str)
{
	return std::wstring(str.begin(), str.end());
}

class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetClassName() noexcept;
		static HINSTANCE GetHInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wClassName = L"WindowClass";
		static WindowClass wClass;
		HINSTANCE hInstance;
	};
public:
	Window(const wchar_t* name, int32 width, int32 height);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string name);
	static bool ProcessMessages(int32* exitCode) noexcept;
	Win32Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard keyboard;
	Mouse mouse;
private:
	int32 width;
	int32 height;
	HWND hwnd;
	std::unique_ptr<Win32Graphics> pGraphics;
};