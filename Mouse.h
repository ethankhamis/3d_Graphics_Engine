#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Wnd;
public:
	struct Raw_Delta { int x, y; };
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

		Event(TypeName type, const Mouse& p) noexcept
			:
			type(type),
			left_Pressed(p.left_Pressed),
			right_Pressed(p.right_Pressed),
			x(p.x),
			y(p.y)
		{}
	public:
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
	bool Inside_Window_Check() const noexcept;
	bool Left_Pressed() const noexcept;
	bool Right_Pressed() const noexcept;
	bool Middle_Pressed() const noexcept;
	std::optional<Mouse::Event> Read() noexcept;
	bool Empty() const noexcept
	{
		return buffer.empty();
	}
	void Clear() noexcept;
private:
	void Mouse_Pos_Change(int x_, int y_) noexcept;
	void Middle_Mouse_Pressed(int x, int y) noexcept;
	void Middle_Mouse_Released(int x, int y) noexcept;
	void Left_Pressed(int x, int y) noexcept;
	void Left_Released(int x, int y) noexcept;
	void Right_Pressed(int x, int y) noexcept;
	void Right_Released(int x, int y) noexcept;
public:
	std::optional<Raw_Delta> Access_Raw_Data() noexcept;
public:
	void Raw_Mouse_Enable() noexcept;
	void Raw_Mouse_Disable() noexcept;
	bool Raw_Mouse_Status() const noexcept;
private:
	void Wheel_Up(int x, int y) noexcept;
	void Wheel_Down(int x, int y) noexcept;
	void Wheel_Delta(int x, int y, int delta) noexcept;
public:
	void Buffer_AdjustSize() noexcept;
	void ApplyRawDelta(int delta_x, int delta_y) noexcept;
	void RawBuffer_ReduceSize() noexcept;
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
	std::queue<Raw_Delta> Raw_Delta_Buffer;
private:
	int wheelDelta = 0;
	bool Raw_Status = false;

};