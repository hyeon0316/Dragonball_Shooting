

#include "CTimer.h"

CTimer::CTimer()
{
	m_nStartTime = 0;
}

CTimer::~CTimer()
{
}

void CTimer::start()
{
	m_nStartTime = timeGetTime();
}

int CTimer::time()
{
	return (timeGetTime() - m_nStartTime); //timeGetTime:부팅시간제외, 윈도우시작후 흐른 시간
}

bool CTimer::elapsed(int& stime, int interval) //stime: 다시 시작한 시간
{
	int current_time = time();
	if (current_time >= stime + interval)
	{
		stime = current_time; //다시 현재시간으로 바꿔줌
		return true;
	}
	else
		return false;
}