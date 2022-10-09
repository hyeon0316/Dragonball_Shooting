#ifndef __gobject_h__
#define __gobject_h__

#include <ddraw.h>
#include "CSprite.h" // ���⵵ �������� sprite �տ� c �ϳ� �ٿ��ָ� ���� x
#include "CTimer.h" // ���� �����ϴ�.

class CGObject {
public:
	float m_x; //��ü�� ��ǥ x, y
	float m_y;
protected:
	int m_nFrameInteval; //������ ����
	int m_nCurrentFrame; //���� ������
	int m_nLastFrameTime; //������������
	bool m_blsLive;
	CSprite* m_pSprite;
	CTimer* m_pTimer;
	

public:
	CGObject();
	~CGObject();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval); //��ü ����
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface, bool usingColor);
	void DrawItem(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface); //������ �׷��� ��
	void DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface); //�ش� �������� ���� ��������Ʈ �׷���
	bool IsLive();
	void Kill();
	int GetX();
	int GetY();
};
#endif#pragma once
