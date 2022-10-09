#ifndef __enemy_h__
#define __enemy_h__

#include "CGObject.h"

class CEnemy : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	int m_speedx;
	int m_speedy;


public:
	CEnemy();
	~CEnemy();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
};
#endif#pragma once
