#include "FirstBoss.h"
#include "Dsutil.h"
#include "BossMissile.h"
#include "StageManager.h"

extern HSNDOBJ Sound[17];
extern StageManager stageManager;

void FirstBoss::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	Boss::Initialize(false, pSprite, x, y, currentFrame, frameInterval, moveInterval);
	m_Hp = 20;
	m_Dir = 2;
	m_IsEntry = true;
}

void FirstBoss::ExecutePattern()
{
	if (!m_IsLive)
		return;

	SetPattern(1);
}


void FirstBoss::TakeDamage(int value)
{
	m_Hp -= value;
	SetExplosionEffect();
	if (m_Hp <= 0)
	{
		Kill();
	}
}

void FirstBoss::Kill()
{
	stageManager.SetFirstBoss(false);
	stageManager.SetSecondBoss(true);
	SndObjPlay(Sound[6], NULL);
	Enemy::Kill();
}