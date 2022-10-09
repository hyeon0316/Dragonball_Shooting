

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
	return (timeGetTime() - m_nStartTime); //timeGetTime:���ýð�����, ����������� �帥 �ð�
}

bool CTimer::elapsed(int& stime, int interval) //stime: �ٽ� ������ �ð�
{
	int current_time = time();
	if (current_time >= stime + interval)
	{
		stime = current_time; //�ٽ� ����ð����� �ٲ���
		return true;
	}
	else
		return false;
}