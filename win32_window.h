#pragma once
#include "hwindows.h"
#include "types.h"
#include "win32_graphics.h"
#include "keyboard.h"
#include "mouse.h"
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
		static const wchar_t* GetName() noexcept;
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
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	const wchar_t* wName;
	HWND hwnd;
	int32 wWidth;
	int32 wHeight;
	std::unique_ptr<Win32Graphics> pGraphics;
public:
	void setTitle(const std::string text);
	bool setSize(const int32 x, const int32 y);
	bool setPos(const int32 x, const int32 y);
	bool shouldClose();
	static bool processMessages(int32* exitCode) noexcept;
	Win32Graphics& gfx();
public:
	Mouse mouse;
	Keyboard keyboard;
};