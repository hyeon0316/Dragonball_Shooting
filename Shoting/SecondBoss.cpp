#include "SecondBoss.h"
#include "Dsutil.h"
#include "StageManager.h"
#include "Animation.h"
#include <dsound.h>

extern HSNDOBJ Sound[17];
extern StageManager stageManager;
extern Animation g_Boss3Intro;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;

void SecondBoss::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	Boss::Initialize(isLive, pSprite, x, y, currentFrame, frameInterval, moveInterval);
	m_MaxHp = 20;
	m_Hp = m_MaxHp;
	m_IsEntry = true;
	m_Dir = 2;
}

void SecondBoss::ExecutePattern()
{
	if (!m_IsLive)
		return;

	if (m_Hp > m_MaxHp * 0.5)
	{
		SetPattern(1);
	}
	else
	{
		SetPattern(2);
	}
}

void SecondBoss::TakeDamage(int value)
{
	m_Hp -= value;
	if (m_Hp <= 0)
	{
		Kill();
	}
}

void SecondBoss::Kill()
{
	g_Boss3Intro.Revive();
	stageManager.SetSecondBoss(false);
	SndObjStop(Sound[0]);
	SndObjPlay(Sound[7], NULL); 
	SndObjPlay(Sound[14], NULL); 
	SndObjPlay(Sound[15], DSBPLAY_LOOPING);
	Enemy::Kill();
	while (true)
	{
		if (g_Boss3Intro.IsDrawEnd(g_IpSecondarySurface))
		{
			stageManager.SetLastBoss(true);
			break;
		}
	}
}

