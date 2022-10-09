#pragma once
#ifndef __mymissile_h__
#define __mymissile_h__

#include "CGObject.h"

class CMyMissile : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;

public:
	CMyMissile();
	~CMyMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);

	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
};
#endif

