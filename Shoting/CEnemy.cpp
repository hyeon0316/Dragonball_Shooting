#include "CEnemy.h" 

CEnemy::CEnemy() {
	m_speedy = 2;

}

CEnemy::~CEnemy() {

}

void CEnemy::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CEnemy::Move()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{

		if (m_y < 0) //¸ð´ÏÅÍÁÂÇ¥
		{
			m_speedy = 2;
			m_x -= 60;
		}
		if (m_y > 768)
		{
			m_speedy = -2;
			m_x -= 60;
		}
		m_y += m_speedy;

		if (m_x < 80)
			m_x = 1200;
	}
}

void CEnemy::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface,true);
}


#include "CEnemyMissile.h"

CEnemyMissile::CEnemyMissile() {

}

CEnemyMissile::~CEnemyMissile() {

}

void CEnemyMissile::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CEnemyMissile::Move()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x -= 5;
		if (m_x < 0)
			m_blsLive = false;	
	}
}

void CEnemyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface,true);
}
