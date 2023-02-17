#include "Timer.h"


Timer::Timer() noexcept
{
	fin = std::chrono::steady_clock::now(); // intiialise time to current time (most recent)
	beg = fin; // hold time of the construction of the timer
}

float Timer::MarkTime() noexcept
{
	const std::chrono::steady_clock::time_point
		initial = fin; // tzke previous final time
	fin = std::chrono::steady_clock::now(); // update final time to now
	const std::chrono::duration<float>
		delta = fin - initial; // find difference between final and initial time
	return delta.count(); //return the delta in seconds

}

float Timer::Duration_Between_Start_to_Now() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::
		now() - beg).count(); // total duration from the beginning of the runtime
}

float Timer::Duration_Between_Frame_to_Now() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::
		now() - fin).count(); // returns current time in program
}
