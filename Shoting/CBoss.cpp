#include "CBoss.h" 
extern int nBossFrame;
extern CTimer g_Timer;
int g_Boss2Move;
int g_Boss3Move;
int g_entireTime;
int g_entireTime2;
bool motionOn = false;
bool boss2Missile = false;
bool boss3Missile = false;
bool startMove = false;
bool skillAtkEn = false;

bool boss2MissileAtk = false;
bool boss3MissileAtk = false;

CBoss::CBoss() {
	m_speedx = 2;
	m_speedy = 2;
}

CBoss::~CBoss() {

}

void CBoss::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CBoss::Move()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		if (!startMove)//화면밖에서 천천히 인게임으로 들어옴
		{
			--m_x;
			nBossFrame = 0;
		}
		if (m_x == 1000)
		{
			m_x = 1000;
			startMove = true;
			skillAtkEn = true;
		}

		if (startMove)//위 아래 번갈아 가면서 이동
		{
			if (m_y <= 50)
			{
				m_speedy = 2;
				m_y = 50;
			}
			if (m_y >= 768)
			{
				m_speedy = -2;
				m_y = 768;
			}
			++m_y;
		}
	}
}

void CBoss::Move2()//2번째 패턴
{
	if (!m_blsLive)
		return;

		if (g_Timer.elapsed(g_entireTime, 1500)) //정해진 3자리 값 중 랜덤 이동
		{
			int arr[3] = { 100,425,700 };
			int i = rand() % 3;
			m_y = arr[i];
			nBossFrame = 2;
			g_Boss2Move = g_Timer.time();
			motionOn = false;
			boss2Missile = false;
		}
		if (!motionOn)
		{
			if (g_Timer.elapsed(g_Boss2Move, 1000))
			{
				motionOn = true;
				boss2Missile = true;
				boss2MissileAtk = true;
			}
		}
		if (motionOn)//3자리 중 하나로 이동하여 멈췄다가 에너지파 발사
		{
			nBossFrame = 3;
			if (g_Timer.elapsed(g_Boss2Move, 1500))
				boss2Missile = false;
		}
}

void CBoss::Move3()//3번째 패턴
{
	if (!m_blsLive)
		return;

	if (g_Timer.elapsed(g_entireTime2, 1500))
	{
		m_x = 1200;
		m_y = 384;
		nBossFrame = 4;
		g_Boss3Move = g_Timer.time();
		boss3Missile = false;
	}
	if (g_Timer.elapsed(g_Boss3Move, 1000))
	{
		boss3Missile = true;
		boss3MissileAtk = true;
	}
}
void CBoss::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface, true);
}

void CBoss::DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame) {
	CGObject::DrawFrame(m_x, m_y, Frame, lpSurface);
}

#include "CBossMissile.h"

CBossMissile::CBossMissile() {
	m_speedx = 2;
	m_speedy = 2;
}

CBossMissile::~CBossMissile() {

}

void CBossMissile::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CBossMissile::Move()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		if (startMove) {
			m_y += m_speedy;
			m_x -= 5;
			if (m_y > 800)//화면 밖으로 벗어날 시 삭제
				m_blsLive = false;
			else if (m_x < 50)
				m_blsLive = false;
		}
	}
}
void CBossMissile::Move3()
{
	if (!m_blsLive)
		return;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_y += 5;
		if (m_y > 800)
			m_blsLive = false;
	}
}
void CBossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface,true);
}
