#include "Enemy.h" 
#include "CAnimation.h"
#include "define.h"

extern CAnimation explosionEffects[MAX_EXPLODES];

void Enemy::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	GameObject::Initialize(isLive, pSprite, x, y, currentFrame, frameInterval);
	m_MoveInterval = moveInterval;
	m_LastMoveTime = Timer::Now();
}

void Enemy::Kill()
{
	SetExplosionEffect();
	GameObject::Kill();
}

void Enemy::SetExplosionEffect()
{
	for (int i = 0; i < MAX_EXPLODES; i++)
	{
		if (!explosionEffects[i].IsLive())
		{
			explosionEffects[i].IsLive();
			explosionEffects[i].SetXY(m_X, m_Y);
			break;
		}
	}
}

void Enemy::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(m_X, m_Y, lpSurface, true);
}

