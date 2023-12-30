#include "LastBoss.h"
#include "CAnimation.h"
#include "StageManager.h"
#include "Dsutil.h"
#include "GameEnum.h"

extern CAnimation g_Ending;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern StageManager stageManager;
extern HSNDOBJ Sound[17];
extern EMod curMod;
extern bool m_bIntroFirst;

void LastBoss::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	Boss::Initialize(isLive, pSprite, x, y, currentFrame, frameInterval, moveInterval);
	m_MaxHp = 30;
	m_Hp = m_MaxHp;
	m_IsEntry = true;
	m_Dir = 2;
}

void LastBoss::ExecutePattern()
{
	if (!m_IsLive)
		return;

	if (m_Hp >= m_MaxHp * 0.7)
	{
		SetPattern(1);
	}
	else if(m_Hp >= m_MaxHp * 0.4)
	{
		SetPattern(2);
	}
	else
	{
		SetPattern(3);
	}
}

void LastBoss::TakeDamage(int value)
{
	m_Hp -= value;
	if (m_Hp <= 0)
	{
		Kill();
	}
}

void LastBoss::Kill()
{
	SndObjStop(Sound[15]);
	SndObjPlay(Sound[16], NULL);
	stageManager.SetLastBoss(false);
	g_Ending.Revive();
	Enemy::Kill();
	while (true)
	{
		if (g_Ending.IsDrawEnd(g_IpSecondarySurface))
		{
			m_bIntroFirst = true;
			curMod = EMod::Intro;
			break;
		}
	}
}