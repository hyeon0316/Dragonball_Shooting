#include <vector>
#include "PlayerMissile.h"
#include "define.h"
#include "Enemy.h"
#include "Player.h"
#include "Animation.h"
#include "FirstBoss.h"
#include "SecondBoss.h"
#include "LastBoss.h"

extern Player player;
extern Animation explosionEffects[MAX_EXPLODES];
extern vector<Enemy*> enemies;

void PlayerMissile::Initialize(Sprite* pSprite, int x, int y, int currentFrame, int framelnterval, int moveInterval)
{
	GameObject::Initialize(false, pSprite, x, y, currentFrame, framelnterval);
	m_MoveInterval = moveInterval;
	m_LastMoveTime = Timer::Now();
}

void PlayerMissile::Move()
{
	if (!m_IsLive)
		return;

	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{
		for (int i = 0; i < enemies.size(); i++) //피격 체크
		{
			if (enemies[i]->IsLive())
			{
				if (abs(enemies[i]->GetX() - m_X) < 30)
				{
					if (abs(enemies[i]->GetY() - m_Y) < 30)
					{
						enemies[i]->TakeDamage(1);
						SetExplosionEffect();
						Kill();
					}
				}
			}
		}

		m_X += 8; 
		if (m_X > 1330)
		{
			Kill();
		}
	}
}

void PlayerMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::DrawTargetFrame(m_X, m_Y, m_CurrentFrame, lpSurface);
}

void PlayerMissile::SetExplosionEffect()
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