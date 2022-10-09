#pragma once
#ifndef __random_h__
#define __random_h__

#include <Windows.h>

class CRandom
{
private:
	int m_nCount;
	void sowseed();

public:
	CRandom();
	~CRandom();
	int number(int m, int n);
};
#endif