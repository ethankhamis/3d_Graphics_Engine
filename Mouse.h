#pragma once
#include <queue>


class Mouse
{
	friend class Window;
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
public:
	struct Event
	{
		enum class TypeName
		{
			LPress,
			LRelease,
			RPress,
			MMouse,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Inside,
			Outside,
			Unrecognised
		};
	public:
		Event() noexcept
			:
			type(TypeName::Unrecognised),
			left_Pressed(false),
			right_Pressed(false),
			x(0),
			y(0)
		{}
		Event(TypeName type, const Mouse& p) noexcept
			:
			type(type),
			left_Pressed(p.left_Pressed),
			right_Pressed(p.right_Pressed),
			x(p.x),
			y(p.y)
		{}
	public:
		bool Valid() const noexcept
		{
			return type != TypeName::Unrecognised;
		}
		TypeName Type() const noexcept
		{
			return type;
		}
		std::pair<int, int> Position() const noexcept //return type will be changed after adding vectors/matrices
		{
			return{ x,y };
		}
		int Position_x() const noexcept
		{
			return x;
		}
		int Position_y() const noexcept
		{
			return y;
		}
		bool Left_Pressed() const noexcept
		{
			return left_Pressed;
		}
		bool Right_Pressed() const noexcept
		{
			return right_Pressed;
		}
	private:
		int x;
		int y;
		bool left_Pressed;
		bool right_Pressed;
		enum TypeName type;
	};
public:
	std::pair<int, int> Position() const noexcept; //return type will be changed after adding vectors/matrices
	int Position_x() const noexcept;
	int Position_y() const noexcept;
	bool Inside_Window() const noexcept;
	bool Left_Pressed() const noexcept;
	bool Right_Pressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool Empty() const noexcept
	{
		return buffer.empty();
	}
	void Clear() noexcept;
public:
	void Mouse_Pos_Change(int x_, int y_) noexcept;
	void Middle_Mouse_Pressed(int x, int y) noexcept;
	void Middle_Mouse_Released(int x, int y) noexcept;
	void Left_Pressed(int x, int y) noexcept;
	void Left_Released(int x, int y) noexcept;
	void Right_Pressed(int x, int y) noexcept;
	void Right_Released(int x, int y) noexcept;
public:
	void Wheel_Up(int x, int y) noexcept;
	void Wheel_Down(int x, int y) noexcept;
	void Buffer_ReduceSize() noexcept;
public:
	void Mouse_Inside() noexcept;
	void Mouse_Outside() noexcept;
private:
	int x;
	int y;
	bool left_Pressed = false;
	bool right_Pressed = false;
	bool middle_mouse_Pressed = false;
	bool inside_window = false;
	static constexpr unsigned int bufferSize = 16u;
	std::queue<Event> buffer;
};