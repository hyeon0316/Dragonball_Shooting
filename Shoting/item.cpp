#include "item.h"
#include "myship.h"
extern CMyShip g_MyShip;
#include <time.h>
extern CSprite g_BarrierEffectSprite;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
int barrierTime = 0;

Item::Item() {
	m_speedx= 4;
	m_speedy= 4;
}

Item::~Item()
{

}
void Item::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, n_currentItem, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void Item::Move()//아이템 랜덤 이동
{
	if (!m_blsLive)
		return;

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{	
		if (m_x > 1366)
		{
			m_x = 1366;
			m_speedx = rand() % 8 - 8;
			m_speedy = rand() % 8 - 4;
		}
		if (m_x < 0)
		{
			m_x = 0;
			m_speedx = rand() % 4 + 1;
			m_speedy = rand() % 8 - 4;
		}
		if (m_y > 768)
		{
			m_y = 768;
			m_speedx = rand() % 8 - 4;
			m_speedy = rand() % 8 - 8;
		}
		if (m_y < 0)
		{
			m_y = 0;
			m_speedx = rand() % 8 - 4;
			m_speedy = rand() % 4 + 1;
		}
		m_x += m_speedx;
		m_y += m_speedy;
	}
}

void Item::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::DrawItem(m_x, m_y, lpSurface);
}

void Item::HpUp(int plusHp)
{
	g_MyShip.hp += plusHp;
}
void Item::MpUp(int plusMp)
{
	g_MyShip.mp += plusMp;
}

