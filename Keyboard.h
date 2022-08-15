#pragma once
#include <queue>
#include <bitset>
#define NullUnsigned 0u

struct Keyboard
{
	friend class Wnd;

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	struct Event
	{
	public:
		Event():type(TypeName::Unrecognised),keycode(NullUnsigned) {}
	public:
		enum class TypeName { Press, Release, Unrecognised };
	private:
		TypeName type;
		unsigned char keycode;
	public:
		Event(TypeName type, unsigned char keycode) noexcept:type(type),keycode(keycode){}

	public:
		unsigned char Keycode_Get() const noexcept
		{
			return keycode;
		}
		bool Valid() const noexcept
		{
			return type != TypeName::Unrecognised;
		}
		bool Released() const noexcept
		{
			return type == TypeName::Release;
		}
		bool Pressed() const noexcept
		{
			return type == TypeName::Press;
		}

	};


	// char events
	char Char_Read() noexcept;
	bool Char_Empty() const noexcept;
	void Char_Flush() noexcept;
	void Flush() noexcept;

	// key events
	bool Key_Pressed(unsigned char keycode) const noexcept;
	Event Key_Read() noexcept;
	bool Key_Empty() noexcept;
	void Key_Flush() noexcept;

private:
	void Key_onPress(unsigned char keycode) noexcept;
	void Key_OnRelease(unsigned char keycode) noexcept;
	void Char_OnPress(char character) noexcept;
	void State_Clear() noexcept;

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool AutoRepeat_State = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> KeyBuffer;
	std::queue<char> CharBuffer;
	template<typename T>
	static void Buffer_ReduceSize(std::queue<T>& b) noexcept;

public:
	void AutoRepeat_Enable() noexcept;
	void AutoRepeat_Disable() noexcept;
	bool AutoRepeatState() const noexcept;
};

