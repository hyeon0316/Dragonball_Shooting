#pragma once
#include"Enemy.h"

class NormalEnemy : public Enemy
{
public:
	NormalEnemy() = default;
	~NormalEnemy() = default;
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval) override;
	void TakeDamage(int value) override;
	void ExecutePattern() override;
	void Kill() override;
};