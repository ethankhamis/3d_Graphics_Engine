#include "Timer.h"


Timer::Timer() noexcept
{
	fin = std::chrono::steady_clock::now(); // intiialise time to current time (most recent)
}

float Timer::MarkTime() noexcept
{
	const std::chrono::steady_clock::time_point
		initial = fin;
	fin = std::chrono::steady_clock::now();
	const std::chrono::duration<float>
		delta = fin - initial;
	return delta.count(); //find difference in time from initial to final

}

float Timer::Duration() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::
		now() - fin).count(); // returns current time in program
}
