#pragma once
#include <chrono>

using namespace std;
using namespace chrono;

class Timer
{
public:
	Timer();
	~Timer() = default;
	static void Start();
	static system_clock::time_point Now();
	static bool Elapsed(system_clock::time_point& time, int interval);
	static float GetDeltaTime();
	static void UpdateDeltatime();

private:
	system_clock::time_point m_StartTime;
	system_clock::time_point m_PrevFrameTime;
	float m_DeltaTime;
};
