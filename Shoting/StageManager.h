#pragma once
class StageManager
{
public:
	StageManager() = default;
	~StageManager() = default;
	void Init();
	void SpawnFirstBoss();
	bool IsFirstBoss() const;
	void SetFirstBoss(bool isActive);
	bool IsSecondBoss() const;
	void SetSecondBoss(bool isActive);
	bool IsLastBoss() const;
	void SetLastBoss(bool isActive);
private:
	int m_NormalEnemyCount;
	bool m_IsFirstBoss;
	bool m_IsSecondBoss;
	bool m_IsLastBoss;
};

inline void StageManager::SetFirstBoss(bool isActive)
{
	m_IsFirstBoss = isActive;
}

inline bool StageManager::IsFirstBoss() const
{
	return m_IsFirstBoss;
}

inline void StageManager::SetSecondBoss(bool isActive)
{
	m_IsSecondBoss = isActive;
}


inline bool StageManager::IsSecondBoss() const
{
	return m_IsSecondBoss;
}

inline void StageManager::SetLastBoss(bool isActive)
{
	m_IsLastBoss = isActive;
}

inline bool StageManager::IsLastBoss() const
{
	return m_IsLastBoss;
}