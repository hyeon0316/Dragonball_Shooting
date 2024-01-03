#include "Timer.h"

static Timer g_Timer;

Timer::Timer()
	: m_StartTime(system_clock::now())
	 ,m_PrevFrameTime(system_clock::now())
{
}

void Timer::Start()
{
	g_Timer.m_StartTime = system_clock::now();
}

/// <summary>
/// ������ ���� �� ���� �ð�
/// </summary>
system_clock::time_point Timer::Now()
{
	return system_clock::time_point(system_clock::now() - g_Timer.m_StartTime);
}

/// <summary>
/// Ư�� �ð� ���� interval ��ŭ �������� Ȯ��
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
