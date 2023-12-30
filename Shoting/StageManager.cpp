#include"StageManager.h"
#include "define.h"

void StageManager::Init()
{
	m_NormalEnemyCount = MAX_XENEMYS * MAX_YENEMYS;
	m_IsFirstBoss = false;
	m_IsSecondBoss = false;
	m_IsLastBoss = false;
}

void StageManager::SpawnFirstBoss()
{
	m_NormalEnemyCount--;
	if (m_NormalEnemyCount == 0)
	{
		m_IsFirstBoss = true;
	}
}
