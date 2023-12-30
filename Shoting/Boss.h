#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
protected:
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	bool GetEntryStatus() const;
	void SetPattern(int order);
	bool m_IsEntry;
private:
	void FirstPattern();
	void SecondPattern();
	void LastPattern();
	bool m_CanSkill;
	system_clock::time_point m_TeleportTime;
	system_clock::time_point m_SkillTime;
};


inline bool Boss::GetEntryStatus() const
{
	return m_IsEntry;
}