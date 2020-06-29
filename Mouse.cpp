#include "Mouse.h"

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}
bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}
bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}
int32 Mouse::GetPosX() const noexcept
{
	return x;
}
int32 Mouse::GetPosY() const noexcept
{
	return y;
}
Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

bool Mouse::IsEmpty() const noexcept
{
	return buffer.empty();
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::MouseMove(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}
void Mouse::MouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::MouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::LeftPressed(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPressed, *this));
	TrimBuffer();
}
void Mouse::LeftReleased(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::LReleased, *this));
	TrimBuffer();
}
void Mouse::RightPressed(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPressed, *this));
	TrimBuffer();
}
void Mouse::RightReleased(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::RReleased, *this));
	TrimBuffer();
}

void Mouse::MiddlePressed(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::MPressed, *this));
	TrimBuffer();
}
void Mouse::MiddleReleased(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::MReleased, *this));
	TrimBuffer();
}

void Mouse::WheelUp(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}
void Mouse::WheelDown(int32 newX, int32 newY) noexcept
{
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}
void Mouse::WheelDelta(int32 newX, int32 newY, int32 delta) noexcept
{
	x = newX;
	y = newY;

	wheelDeltaCarry += delta;
	while (wheelDeltaCarry >= wheelDelta)
	{
		wheelDeltaCarry -= wheelDelta;
		WheelDown(x, y);
	}
	while (wheelDeltaCarry <= -wheelDelta)
	{
		wheelDeltaCarry += wheelDelta;
		WheelUp(x, y);
	}
}
void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}