#ifndef __BossMissile_h__
#define __BossMissile_h__

#include "CGObject.h"

class CBossMissile : public CGObject
{
public:
	float m_speedx;
	float m_speedy;
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	CBossMissile();
	~CBossMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
	void Move3();
};
#endif