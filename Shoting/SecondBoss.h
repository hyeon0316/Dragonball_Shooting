#pragma once
#include"Boss.h"

class SecondBoss : public Boss
{
public:
	SecondBoss() = default;
	~SecondBoss() = default;
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval) override;
	void TakeDamage(int value) override;
	void ExecutePattern() override;
	void Kill() override;
};


