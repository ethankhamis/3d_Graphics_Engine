#include "Mouse.h"

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

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event ev = buffer.front();
		buffer.pop();
		return ev;
	}
	else
	{
		return Mouse::Event();
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
	Buffer_ReduceSize();
}

void Mouse::Middle_Mouse_Pressed(int x, int y) noexcept
{
	middle_mouse_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::MMouse, *this));
	Buffer_ReduceSize();
	
}

void Mouse::Middle_Mouse_Released(int x, int y) noexcept
{
	middle_mouse_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::MMouse, *this));
	Buffer_ReduceSize();
}

void Mouse::Left_Pressed(int x, int y) noexcept
{
	left_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::LPress, *this));
	Buffer_ReduceSize();
}

void Mouse::Left_Released(int x, int y) noexcept
{
	left_Pressed = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::LPress, *this));
	Buffer_ReduceSize();
}

void Mouse::Right_Pressed(int x, int y) noexcept
{
	right_Pressed = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::RPress, *this));
	Buffer_ReduceSize();
}

void Mouse::Right_Released(int x, int y) noexcept
{
	right_Pressed = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::RPress, *this));
	Buffer_ReduceSize();
}

void Mouse::Wheel_Up(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::TypeName::WheelUp, *this));
	Buffer_ReduceSize();
}

void Mouse::Wheel_Down(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::TypeName::WheelUp, *this));
	Buffer_ReduceSize();
}

void Mouse::Buffer_ReduceSize() noexcept
{
	while (buffer.size() > bufferSize){buffer.pop();}
}

void Mouse::Mouse_Inside() noexcept
{
	inside_window = true;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::Inside, *this));
	Buffer_ReduceSize();
}

void Mouse::Mouse_Outside() noexcept
{
	inside_window = false;
	buffer.push(Mouse::Event(Mouse::Event::TypeName::Inside, *this));
	Buffer_ReduceSize();
}
