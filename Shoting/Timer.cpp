#include "Timer.h"

static Timer g_Timer;

Timer::Timer()
	: m_StartTime(system_clock::now())
	 ,m_PrevFrameTime(system_clock::now())
	 ,m_DeltaTime(0.001f)
{
}

void Timer::Start()
{
	g_Timer.m_StartTime = system_clock::now();
}

/// <summary>
/// 게임이 켜진 뒤 지난 시간
/// </summary>
system_clock::time_point Timer::Now()
{
	return system_clock::time_point(system_clock::now() - g_Timer.m_StartTime);
}

/// <summary>
/// 특정 시간 기준 interval 만큼 지났는지 확인
/// </summary>
bool Timer::Elapsed(system_clock::time_point& time, int interval) 
{
	const system_clock::time_point curTime = Now();
	const milliseconds intervalTime(interval);
	if (curTime >= time + intervalTime)
	{
		time = curTime;
		return true;
	}

	return false;
}

float Timer::GetDeltaTime()
{
	return g_Timer.m_DeltaTime;
}

void Timer::UpdateDeltatime()
{
	const system_clock::time_point curTime = system_clock::now();
	const duration<float> frameTime = curTime - g_Timer.m_PrevFrameTime;
	g_Timer.m_PrevFrameTime = curTime;
	g_Timer.m_DeltaTime = frameTime.count();
}