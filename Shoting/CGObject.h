#ifndef __gobject_h__
#define __gobject_h__

#include <ddraw.h>
#include "CSprite.h" // 여기도 마찬가지 sprite 앞에 c 하나 붙여주면 오류 x
#include "CTimer.h" // 위와 같습니다.

class CGObject {
public:
	float m_x; //객체의 좌표 x, y
	float m_y;
protected:
	int m_nFrameInteval; //프레임 간격
	int m_nCurrentFrame; //현재 프레임
	int m_nLastFrameTime; //마지막프레임
	bool m_blsLive;
	CSprite* m_pSprite;
	CTimer* m_pTimer;
	

public:
	CGObject();
	~CGObject();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval); //객체 생성
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface, bool usingColor);
	void DrawItem(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface); //아이템 그려줄 때
	void DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface); //해당 프레임을 가진 스프라이트 그려줌
	bool IsLive();
	void Kill();
	int GetX();
	int GetY();
};
#endif#pragma once
