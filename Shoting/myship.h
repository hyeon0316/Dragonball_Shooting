#pragma once
#ifndef __myship_h__
#define __myship_h__

#include "CGObject.h"

class CMyShip : public CGObject
{
private:
	int m_nLastMoveTime;
	int m_nMoveInterval;


public:
	CMyShip();
	~CMyShip();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval);
	void DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame);
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void Left();
	void Right();
	void Down();
	void Up();
	int hp;
	int mp;
};
#endif