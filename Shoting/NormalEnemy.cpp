#include "NormalEnemy.h"
#include "Item.h"
#include "StageManager.h"
#include "EnemyMissile.h"

extern Item dropItem;
extern StageManager stageManager;
extern EnemyMissile enemyMissiles[MAX_ENEMY_MISSILES];

int randAttack = 0;
void NormalEnemy::Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	m_Hp = 1;
	Enemy::Initialize(isLive, pSprite, x, y, currentFrame, frameInterval, moveInterval);
}

void NormalEnemy::TakeDamage(int value)
{
	m_Hp -= value;
	if (m_Hp <= 0)
	{
		Kill();
	}
}

void NormalEnemy::ExecutePattern()
{
	if (!m_IsLive)
		return;

	//이동
	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{
		if (m_Y < 0)
		{
			m_Dir = 2;
		}
		if (m_Y > SCREEN_HEIGHT)
		{
			m_Dir = -2;
		}
		m_Y += m_Dir * Timer::GetDeltaTime();
		m_X -= 60 * Timer::GetDeltaTime();

		if (m_X < 80)
			m_X = 1200;
	}

	//공격
	randAttack = rand() % 150;
	if (rand == 0)
	{
		for (int i = 0; i < MAX_ENEMY_MISSILES; i++)
		{
			if (!enemyMissiles[i].IsLive())
			{
				enemyMissiles[i].Revive();
				enemyMissiles[i].SetXY(m_X - 20, m_Y);
				break;
			}
		}
	}

}

void NormalEnemy::Kill()
{
	if (rand() % 10 == 0)
	{
		dropItem.Revive();
		dropItem.SetFrame(rand() % 3); //3개의 아이템중 랜덤
		dropItem.SetXY(m_X, m_Y);
	}
	stageManager.SpawnFirstBoss();
	Enemy::Kill();
}