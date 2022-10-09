#ifndef __enemymissile_h__
#define __enemymissile_h__

#include "CGObject.h"

class CEnemyMissile : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;

public:
	CEnemyMissile();
	~CEnemyMissile();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
};
#endif#
