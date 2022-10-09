#pragma once
#include"CGObject.h"
CGObject::CGObject() {
	m_nCurrentFrame = 0;
	m_blsLive = false;
}

CGObject::~CGObject()
{
}

void CGObject::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval)
{
	m_pSprite = pSprite;
	m_nCurrentFrame = CurrentFrame;
	m_x = x;
	m_y = y;
	m_nFrameInteval = FrameInterval;
	m_blsLive = true;
	m_pTimer = timer;
	m_nLastFrameTime = m_pTimer->time();
}

bool CGObject::IsLive() {
	return m_blsLive;
}

void CGObject::Kill() {
	m_blsLive = false;
}

void CGObject::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface, bool usingColor) {
	if (!m_blsLive)
		return;

	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInteval))
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame(); //이미지를 차례로 보여줌
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpSurface, usingColor);
}

void CGObject::DrawItem(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_blsLive)
		return;
	
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpSurface);
}

void CGObject::DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface) {//해당 프레임 번호의 이미지를 보여줌
	if (!m_blsLive)
		return;
	if (nFrame >= m_pSprite->GetNumberOfFrame())
		nFrame = m_pSprite->GetNumberOfFrame() - 1;
	if (nFrame < 0)
		nFrame = 0;

	m_pSprite->Drawing(nFrame, m_x, m_y, lpSurface);
}

int CGObject::GetX() {
	return m_x;
}

int CGObject::GetY() {
	return m_y;
}