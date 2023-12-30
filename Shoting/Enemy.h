#pragma once
#include "GameObject.h"
#include "define.h"

class Enemy : public GameObject
{
public:
	Enemy() = default;
	~Enemy() = default;
	virtual void Initialize(bool isLive, Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	virtual void ExecutePattern() = 0;
	virtual void TakeDamage(int value) = 0;
	virtual void Kill();
protected:
	void SetExplosionEffect();
	int m_Hp;
	int m_MaxHp;
	int m_MoveInterval;
	float m_Dir;
	system_clock::time_point m_LastMoveTime;
};


