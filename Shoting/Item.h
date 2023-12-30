#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	Item();
	~Item() = default;
	void Move();
	void PlusHp(int plusHp);
	void PlusMp(int plusMp);
	void CheckArea();
	void OnBarrior();
	void SetFrame(int frame);
	int GetCurFrame() const;
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Initialize(Sprite* pSprite, int x, int y, int frameInterval, int moveInterval);
private:
	int m_MoveInterval;
	int m_SpeedX;
	int m_SpeedY;
	system_clock::time_point m_LastMoveTime;
};

inline int Item::GetCurFrame() const
{
	return m_CurrentFrame;
}

inline void Item::SetFrame(int frame)
{
	m_CurrentFrame = frame;
}
