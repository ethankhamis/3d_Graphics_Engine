#include "Mouse.h"
#include <optional>
#include "WinDef.h"

std::pair<int, int> Mouse::Position() const noexcept
{
	return {x,y};
}

int Mouse::Position_x() const noexcept
{
	return x;
}

int Mouse::Position_y() const noexcept
{
	return y;
}

bool Mouse::Inside_Window_Check() const noexcept
{
	return inside_window;
}

bool Mouse::Left_Pressed() const noexcept
{
	return left_Pressed;
}

bool Mouse::Right_Pressed() const noexcept
{
	return right_Pressed;
}

bool Mouse::Middle_Pressed() const noexcept
{
	return middle_mouse_Pressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event ev = buffer.front();
		buffer.pop();
		//O(1) operation
		return ev;
	}
	else
	{
		return {};
	}
}

void Mouse::Clear() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::Mouse_Pos_Change(int x_, int y_) noexcept
{
	x = x_;
	y = y_;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::Move, *this));
	Buffer_AdjustSize();
}

void Mouse::Middle_Mouse_Pressed(int x, int y) noexcept
{
	middle_mouse_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::MMouse, *this));
	Buffer_AdjustSize();
}
void Mouse::Middle_Mouse_Released(int x, int y) noexcept
{
	middle_mouse_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::MMouse, *this));
	Buffer_AdjustSize();
}

void Mouse::Left_Pressed(int x, int y) noexcept
{
	left_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::LPress, *this));
	Buffer_AdjustSize();
}

void Mouse::Left_Released(int x, int y) noexcept
{
	left_Pressed = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::LRelease, *this));
	Buffer_AdjustSize();
}

void Mouse::Right_Pressed(int x, int y) noexcept
{
	right_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::RPress, *this));
	Buffer_AdjustSize();
}

void Mouse::Right_Released(int x, int y) noexcept
{
	right_Pressed = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::RRelease, *this));
	Buffer_AdjustSize();
}

std::optional<Mouse::Raw_Delta> Mouse::Access_Raw_Data() noexcept
{
	if (Raw_Delta_Buffer.empty())
	{
		return std::nullopt;
	}
	const Raw_Delta delta_raw = Raw_Delta_Buffer.front();
	Raw_Delta_Buffer.pop();
	return delta_raw;
}

void Mouse::Raw_Mouse_Enable() noexcept
{
	Raw_Status = true;
}

void Mouse::Raw_Mouse_Disable() noexcept
{
	Raw_Status = false;
}

bool Mouse::Raw_Mouse_Status() const noexcept
{
	return Raw_Status;
}

void Mouse::Wheel_Up(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::TypeName::WheelUp, *this));
	Buffer_AdjustSize();
}

void Mouse::Wheel_Down(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::TypeName::WheelDown, *this));
	Buffer_AdjustSize();
}

void Mouse::Wheel_Delta(int x, int y, int delta) noexcept
{
	wheelDelta += delta;
	//(WHEEL_DELTA DEF AS 120)
	while (WHEEL_DELTA <= wheelDelta) //upwards scrolling
	{
		wheelDelta -= WHEEL_DELTA;
		Wheel_Up(x, y);
	}
	while (-WHEEL_DELTA >= wheelDelta) //account for downwards scrolling
	{
		wheelDelta += WHEEL_DELTA;
		Wheel_Down(x, y);
	}
}

void Mouse::Buffer_AdjustSize() noexcept
{
	while (buffer.size() > bufferSize){buffer.pop();}
}

void Mouse::ApplyRawDelta(int delta_x, int delta_y) noexcept
{
	Raw_Delta_Buffer.push({ delta_x, delta_y });
	Buffer_AdjustSize();
}

void Mouse::RawBuffer_ReduceSize() noexcept
{
	while (Raw_Delta_Buffer.size() > bufferSize)
		Raw_Delta_Buffer.pop();
	
}

void Mouse::Mouse_Inside() noexcept
{
	inside_window = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::Inside, *this));
	Buffer_AdjustSize();
}

void Mouse::Mouse_Outside() noexcept
{
	inside_window = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::Outside, *this));
	Buffer_AdjustSize();
}
