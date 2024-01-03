#include "Boss.h"
#include "BossMissile.h"
#include "Animation.h"
#include "LastBossMissile.h"

extern BossMissile bossMissiles[MAX_BOSS_XMISSILES][MAX_BOSS_YMISSILES];
extern LastBossMissile lastBossMissiles[MAX_LASTBOSS_XMISSILES];
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern Animation bossSecondAttack;
extern bool isHitReady;

int dir[] = { -2,0,2 };
int movePos[3] = { 100,425,700 };

void Boss::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	Enemy::Initialize(isLive, pSprite, x, y, currentFrame, frameInterval, moveInterval);
	m_CanSkill = false;
	m_TeleportTime = Timer::Now();
}
void Boss::FirstPattern()
{
	if (m_IsEntry)
	{
		m_X -= 1;
		if (m_X > 1000)
		{
			m_IsEntry = false;
		}
	}
	else
	{
		if (m_Y <= 50)
		{
			m_Dir = 2;
			m_Y = 50;
		}
		if (m_Y >= SCREEN_HEIGHT)
		{
			m_Dir = -2;
			m_Y = SCREEN_HEIGHT;
		}
		m_Y += m_Dir;

		for (int i = 0; i < MAX_BOSS_XMISSILES; i++)
		{
			for (int j = 0; j < MAX_BOSS_YMISSILES; j++)
			{
				if (!bossMissiles[i][j].IsLive())
				{
					bossMissiles[i][j].Revive();
					bossMissiles[i][j].SetXY(m_X - 80, m_Y);
					bossMissiles[i][j].SetDir(dir[j]);
				}
			}
		}
	}
}

void Boss::SecondPattern()
{
	if (!m_CanSkill)
	{
		if (Timer::Elapsed(m_TeleportTime, 2000))
		{
			int i = rand() % 3;
			m_Y = movePos[i];
			m_CurrentFrame = 2;
			m_SkillTime = Timer::Now();
			m_CanSkill = true;
		}
	}
	else
	{
		if (!isHitReady)
		{
			if (Timer::Elapsed(m_SkillTime, 1000)) 
			{
				isHitReady = true;
				bossSecondAttack.Revive();
				bossSecondAttack.SetXY(m_X - 515, m_Y - 15);
				m_CurrentFrame = 3;
				m_SkillTime = Timer::Now();
			}
		}

		if (Timer::Elapsed(m_SkillTime, 2000))
		{
			bossSecondAttack.Kill();
			m_CanSkill = false;
		}
	}
}

void Boss::LastPattern()
{
	m_X = 1200;
	m_Y = 384;
	m_CurrentFrame = 4;

	for (int i = 0; i < MAX_LASTBOSS_XMISSILES; i++)
	{
		if (!lastBossMissiles[i].IsLive())
		{
			if (rand() % 2 == 0)
			{
				lastBossMissiles[i].Revive();
				lastBossMissiles[i].SetXY(rand() % SCREEN_WIDTH, 0);
			}
		}
	}
}

void Boss::SetPattern(int order)
{
	if (order == 1)
	{
		FirstPattern();
	}
	else if (order == 2)
	{
		SecondPattern();
	}
	else if (order == 3)
	{
		LastPattern();
	}
}