#include "mymissile.h"

CMyMissile::CMyMissile() {
}

CMyMissile::~CMyMissile() {
}

void CMyMissile::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int Framelnterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, Framelnterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CMyMissile::Move()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x += 8; //화면밖 나가는거 방지

		if (m_x > 1330)
		{
			m_blsLive = false;
		}
	}
}

void CMyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface, true);
}