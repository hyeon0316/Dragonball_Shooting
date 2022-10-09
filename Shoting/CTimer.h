#pragma once
#ifndef __timer_h__
#define __timer_h__

#include <Windows.h>

class CTimer
{
private:
	int m_nStartTime;

public:
	CTimer();
	~CTimer();
	void start();
	int time();

	bool elapsed(int& time, int interval);
};

#endif