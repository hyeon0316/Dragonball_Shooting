#include "BossMissile.h"
#include"Animation.h"
#include "Player.h"

extern Animation explosionEffects[MAX_EXPLODES];
extern Player player;

BossMissile::BossMissile()
	: m_MoveInterval(0)
	, m_Dir(0)
{}

void BossMissile::Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	GameObject::Initialize(false, pSprite, x, y, currentFrame, frameInterval);
	m_MoveInterval = moveInterval;
}

void BossMissile::Move()
{
	if (!m_IsLive)
		return;

	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{
		if (player.IsLive())
		{
			if (abs(m_X - player.GetX()) < 30)
			{
				if (abs(m_Y - player.GetY()) < 20)
				{
					player.TakeDamage(1);
					SetExplosionEffect();
					Kill();
				}
			}
		}
		m_Y += m_Dir;
		m_X -= 5;

		if (m_X < 50)
			Kill();
	}
}

void BossMissile::SetExplosionEffect()
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

void BossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::DrawTargetFrame(m_X, m_Y, m_CurrentFrame, lpSurface);
}