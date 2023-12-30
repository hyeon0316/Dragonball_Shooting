#pragma once
#include "Boss.h"

class FirstBoss : public Boss
{
public:
	FirstBoss() = default;
	~FirstBoss() = default;
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval) override;
	void ExecutePattern() override;
	void TakeDamage(int value) override;
	void Kill() override;
};


