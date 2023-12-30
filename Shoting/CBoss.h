#ifndef __Boss_h__
#define __Boss_h__

#include "CGObject.h"

class CBoss : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;
public:
	float m_speedx;
	float m_speedy;
	int m_Enegy;

public:
	CBoss();
	~CBoss();
	void Initialize(Sprite* pSprite, int x, int y, Timer* timer, int CurrentFrame, int Frameinterval, int MoveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame);
	void Move();
	void Move2();
	void Move3();
};
#endif