#pragma once
#include <chrono>

struct Timer
{
public:
	Timer() noexcept;
	float MarkTime() noexcept;
	float Duration() const noexcept;
private:
	std::chrono::steady_clock::time_point fin; //final time
};