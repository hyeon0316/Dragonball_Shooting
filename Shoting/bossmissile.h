// bossmissile.h
#ifndef __BossMissile_h__
#define __BossMissile_h__

#include "gobject.h"

class CBossMissile : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	int m_speedx;

public:
	CBossMissile();
	~CBossMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
};
#endif
