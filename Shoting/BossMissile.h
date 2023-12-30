#pragma once
#include "GameObject.h"

class BossMissile : public GameObject
{
public:
	BossMissile();
	~BossMissile() = default;
	void Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void SetDir(float dir);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Move();

private:
	void SetExplosionEffect();
	float m_Dir;
	int m_MoveInterval;
	system_clock::time_point m_LastMoveTime;
};

inline void BossMissile::SetDir(float dir)
{
	m_Dir = dir;
}
