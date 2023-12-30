#include "EnemyMissile.h"
#include "Player.h"
#include "CAnimation.h"

extern Player player;
extern CAnimation explosionEffects[MAX_EXPLODES];

void EnemyMissile::Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	GameObject::Initialize(false, pSprite, x, y, currentFrame, frameInterval);
	m_MoveInterval = moveInterval;
	m_LastMoveTime = Timer::Now();
}

void EnemyMissile::Move()
{
	if (!m_IsLive)
		return;
	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{
		if (player.IsLive())
		{
			if (abs(m_X - player.GetX()) < 30) //충돌체크
			{
				if (abs(m_Y - player.GetY()) < 20)
				{
					SetExplosionEffect();
					player.TakeDamage(1);
					Kill();
				}
			}
		}

		m_X -= 5 * Timer::GetDeltaTime();
		if (m_X < 0)
			Kill();
	}
}

void EnemyMissile::SetExplosionEffect()
{
	for (int i = 0; i < MAX_EXPLODES; i++)
	{
		if (!explosionEffects[i].IsLive())
		{
			explosionEffects[i].Revive();
			explosionEffects[i].SetXY(m_X, m_Y);
			break;
		}
	}
}

void EnemyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(m_X, m_Y, lpSurface, true);
}