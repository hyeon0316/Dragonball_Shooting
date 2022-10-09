#ifndef __Item_h__
#define __Item_h__

#include "CGObject.h"

class Item : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	int m_speedx;
	int m_speedy;

	int n_currentItem;



public:
	Item();
	~Item();

	void Move();
	void HpUp(int plusHp);
	void MpUp(int plusMp);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int FrameInterval, int MoveInterval);
};
#endif