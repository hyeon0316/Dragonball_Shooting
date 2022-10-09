#include "myship.h"

CMyShip::CMyShip() {
}

CMyShip::~CMyShip() {
}

void CMyShip::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
	hp = 5;
	mp = 0;
}

void CMyShip::Left()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x -= 5;
		if (m_x < 30) //width의 최소범위 30
		{
			m_x = 30;
		}
	}
}

void CMyShip::Right()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x += 5;
		if (m_x > 1366)
		{
			m_x = 1366;
		}
	}
}
void CMyShip::Up()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_y -= 5;
		if (m_y <0)
		{
			m_y = 0;
		}
	}
}
void CMyShip::Down() {
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_y += 5;
		if (m_y > 770)
		{
			m_y = 770;
		}
	}
}


void CMyShip::DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame) {
	CGObject::DrawFrame(m_x, m_y, Frame, lpSurface);
}