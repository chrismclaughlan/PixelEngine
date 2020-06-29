#pragma once
#include "types.h"
#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			LPressed, LReleased,
			RPressed, RReleased,
			MPressed, MReleased,
			WheelUp, WheelDown,
			Move, Enter, Leave,
			Invalid,
		};
	private:
		Type type;
		bool leftPressed;
		bool rightPressed;
		int32 x;
		int32 y;
	public:
		Event() : type(Type::Invalid), leftPressed(false), rightPressed(false), x(-1), y(-1) {}
		Event(Type type, const Mouse& parent) : type(type), leftPressed(parent.leftIsPressed), 
			rightPressed(parent.rightIsPressed), x(parent.x), y(parent.y)
		{}
		Type GetType() const noexcept { return type; }
		bool IsValid() const noexcept { return type != Type::Invalid; }
		bool IsLeftPressed() const noexcept { return leftPressed; }
		bool IsRightPressed() const noexcept { return rightPressed; }
		int32 getX() const noexcept { return x; }
		int32 getY() const noexcept { return y; }
		// return pair<x, y>
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;
	int32 GetPosX() const noexcept;
	int32 GetPosY() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;

private:
	void MouseMove(int32 newX, int32 newY) noexcept;
	void MouseEnter() noexcept;
	void MouseLeave() noexcept;
	void LeftPressed(int32 newX, int32 newY) noexcept;
	void LeftReleased(int32 newX, int32 newY) noexcept;
	void RightPressed(int32 newX, int32 newY) noexcept;
	void RightReleased(int32 newX, int32 newY) noexcept;
	void MiddlePressed(int32 newX, int32 newY) noexcept;
	void MiddleReleased(int32 newX, int32 newY) noexcept;
	void WheelUp(int32 newX, int32 newY) noexcept;
	void WheelDown(int32 newX, int32 newY) noexcept;
	void WheelDelta(int32 newX, int32 newY, int32 delta) noexcept;
	void TrimBuffer() noexcept;
private:
	static constexpr uint32 bufferSize = 16u;
	static constexpr int32 wheelDelta = 120;
	int32 x;
	int32 y;
	bool isInWindow = false;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};