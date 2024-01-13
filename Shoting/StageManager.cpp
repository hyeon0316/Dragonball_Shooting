#include"StageManager.h"
#include "define.h"
#include "Enemy.h"
#include "FirstBoss.h"
#include "SecondBoss.h"
#include "LastBoss.h"

extern vector<Enemy*> enemies;

void StageManager::Init()
{
	m_NormalEnemyCount = MAX_XENEMYS * MAX_YENEMYS;
	m_IsFirstBoss = false;
	m_IsSecondBoss = false;
	m_IsLastBoss = false;
}

void StageManager::SetFirstBoss(bool isActive)
{
	m_IsFirstBoss = isActive;
	if (isActive)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (FirstBoss* boss = dynamic_cast<FirstBoss*>(enemies[i]))
			{
				boss->Revive();
				break;
			}
		}
	}
}

void StageManager::SetSecondBoss(bool isActive)
{
	m_IsSecondBoss = isActive;
	if (isActive)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (SecondBoss* boss = dynamic_cast<SecondBoss*>(enemies[i]))
			{
				boss->Revive();
				break;
			}
		}
	}
}

void StageManager::SetLastBoss(bool isActive)
{
	m_IsLastBoss = isActive;
	if (isActive)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (LastBoss* boss = dynamic_cast<LastBoss*>(enemies[i]))
			{
				boss->Revive();
				break;
			}
		}
	}
}

void StageManager::SpawnFirstBoss()
{
	m_NormalEnemyCount--;
	if (m_NormalEnemyCount == 0)
	{
		SetFirstBoss(true);
	}
}
