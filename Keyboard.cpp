#include "Keyboard.h"

std::optional<char> Keyboard::Char_Read() noexcept
{
	if (CharBuffer.size() > 0u)
	{
		unsigned char char_KeyCode = CharBuffer.front();
		CharBuffer.pop(); return char_KeyCode;
	}
	else
	{
		return {};
	}
}

bool Keyboard::Char_Empty() const noexcept
{
	return CharBuffer.empty();
}

void Keyboard::Char_Flush() noexcept
{
	CharBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	Char_Flush();
	Key_Flush();
}

bool Keyboard::Key_Pressed(unsigned char keycode) const noexcept
{
	return keyStates[keycode];
}

std::optional<Keyboard::Event> Keyboard::Key_Read() noexcept
{
	if (KeyBuffer.size() > 0u)
	{
		Keyboard::Event ev = KeyBuffer.front();
		KeyBuffer.pop();
		return ev;
	}
	else 
	{
		return {};
	}
}

bool Keyboard::Key_Empty() noexcept
{
	return KeyBuffer.empty();
}

void Keyboard::Key_Flush() noexcept
{
	KeyBuffer = std::queue<Event>();
}

void Keyboard::Key_onPress(unsigned char keycode) noexcept
{
	keyStates[keycode] = true;
	KeyBuffer.push(Keyboard::Event(Keyboard::Event::TypeName::Press, keycode));
	Buffer_ReduceSize(KeyBuffer);
}

void Keyboard::Key_OnRelease(unsigned char keycode) noexcept
{
	keyStates[keycode] = false;
	KeyBuffer.push(Keyboard::Event(Keyboard::Event::TypeName::Release, keycode));
	Buffer_ReduceSize(KeyBuffer);
}

void Keyboard::Char_OnPress(char character) noexcept
{
	CharBuffer.push(character);
	Buffer_ReduceSize(CharBuffer);
}

void Keyboard::State_Clear() noexcept
{
	keyStates.reset();
}

void Keyboard::AutoRepeat_Enable() noexcept
{
	AutoRepeat_State = true;
}

void Keyboard::AutoRepeat_Disable() noexcept
{
	AutoRepeat_State = false;
}

bool Keyboard::AutoRepeatState() const noexcept
{
	return AutoRepeat_State;
}

template<typename T>
void Keyboard::Buffer_ReduceSize(std::queue<T>& b) noexcept
{
	while (b.size() > bufferSize) { b.pop(); }
}