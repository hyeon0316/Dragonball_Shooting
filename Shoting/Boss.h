#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
public:
	bool GetEntryStatus() const;
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface) override;
protected:
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void SetPattern(int order);
	bool m_IsEntry;
private:
	void FirstPattern();
	void SecondPattern();
	void LastPattern();
	bool m_CanSkill;
	bool m_IsSecondAttacking;
	system_clock::time_point m_TeleportTime;
	system_clock::time_point m_SkillTime;
};


inline bool Boss::GetEntryStatus() const
{
	return m_IsEntry;
}