#pragma once
#include "GameObject.h"

class LastBossMissile : public GameObject
{
public:
	LastBossMissile();
	~LastBossMissile() = default;
	void Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();

private:
	void SetExplosionEffect();
	int m_MoveInterval;
	system_clock::time_point m_LastMoveTime;
};


