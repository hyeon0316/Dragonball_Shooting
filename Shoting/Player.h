#pragma once
#include<ddraw.h>
#include "GameObject.h"
#include "define.h"

class Player : public GameObject
{
public:
	Player();
	~Player() = default;
	void Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void SetMp(int mp);
	void DrawSkill();
	void PlusMp(int value);
	void SetHp(int value);
	int GetHp() const;
	int GetMp() const;
	int GetMaxHp() const;
	int GetMaxMp() const;
	void TakeDamage(int value);
	bool GetBarrierStatus() const;
	bool CheckBarrierTime();
	void SetBarrier(bool isBarrier);
	void InputKey();
	bool CanSkill();
private:
	void Left();
	void Right();
	void Down();
	void Up();
	void Attack();
	void Skill();
	bool CanMove();
	bool CanAttack();
	int m_Hp;
	int m_MaxHp;
	int m_Mp;
	int m_MaxMp;
	system_clock::time_point m_LastMoveTime;
	system_clock::time_point m_LastAttackTime;
	system_clock::time_point m_BarrierTime;
	int m_MoveInterval;
	bool m_IsAttacking;
	bool m_IsChangeAttack;
	bool m_IsBarrier;
};

inline void Player::SetHp(int value)
{
	m_Hp = value;
}

inline int Player::GetHp() const
{
	return m_Hp;
}

inline int Player::GetMp() const
{
	return m_Mp;
}

inline int Player::GetMaxHp() const
{
	return m_MaxHp;
}

inline int Player::GetMaxMp() const
{
	return m_MaxMp;
}

inline void Player::Left()
{
	m_CurrentFrame = 1;
	m_X -= 5;
	if (m_X < 30) //width의 최소범위 30
	{
		m_X = 30;
	}
}

inline void Player::Right()
{
	m_CurrentFrame = 2;
	m_X += 5;
	if (m_X > 1366)
	{
		m_X = 1366;
	}
}

inline void Player::Up()
{
	m_CurrentFrame = 3;
	m_Y -= 5;
	if (m_Y < 0)
	{
		m_Y = 0;
	}
}

inline void Player::Down()
{
	m_CurrentFrame = 4;
	m_Y += 5;
	if (m_Y > 770)
	{
		m_Y = 770;
	}
}

inline void Player::SetBarrier(bool isBarrier)
{
	m_IsBarrier = isBarrier;
	if (m_IsBarrier)
	{
		m_BarrierTime = Timer::Now();
	}
}

inline bool Player::GetBarrierStatus() const
{
	return m_IsBarrier;
}