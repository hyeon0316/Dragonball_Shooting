#pragma once
#include "GameObject.h"

class PlayerMissile : public GameObject
{
public:
	PlayerMissile() = default;
	~PlayerMissile() = default;
	void Initialize(Sprite* pSprite, int x, int y, int currentFrame, int framelnterval, int moveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();
private:
	void SetExplosionEffect();
	int m_MoveInterval;
	system_clock::time_point m_LastMoveTime;
};

