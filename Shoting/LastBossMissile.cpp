#include "LastBossMissile.h"
#include "define.h"
#include"Animation.h"
#include "Player.h"

extern Animation explosionEffects[MAX_EXPLODES];
extern Player player;

LastBossMissile::LastBossMissile()
	: m_MoveInterval(0)
{}

void LastBossMissile::Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	GameObject::Initialize(false, pSprite, x, y, currentFrame, frameInterval);
	m_MoveInterval = moveInterval;
	m_LastMoveTime = Timer::Now();
}

void LastBossMissile::Move()
{
	if (!m_IsLive)
		return;

	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{
		if (player.IsLive())
		{
			if (abs(m_X - player.GetX()) < 40)
			{
				if (abs(m_Y - player.GetY()) < 50)
				{
					SetExplosionEffect();
					player.TakeDamage(2);
					Kill();
				}
			}
		}

		m_Y += 5;

		if (m_Y > 800)
			Kill();
	}
}

void LastBossMissile::SetExplosionEffect()
{
	for (int i = 0; i < MAX_EXPLODES; i++)
	{
		if (!explosionEffects[i].IsLive())
		{
			explosionEffects[i].InitCurFrame(0);
			explosionEffects[i].Revive();
			explosionEffects[i].SetXY(m_X, m_Y);
			break;
		}
	}
}

void LastBossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(m_X, m_Y, lpSurface, true);
}