#pragma once
#include <chrono>

struct Timer
{
public:
	Timer();
	float MarkTime();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point fin; //final time
};