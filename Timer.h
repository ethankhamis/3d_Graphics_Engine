#pragma once
#include <chrono>

struct Timer
{
public:
	Timer() noexcept;
	float MarkTime() noexcept;
	float Duration_Between_Start_to_Now() const noexcept;
	float Duration_Between_Frame_to_Now() const noexcept;
private:
	std::chrono::steady_clock::time_point fin; //final time
	std::chrono::steady_clock::time_point beg; //start time
};