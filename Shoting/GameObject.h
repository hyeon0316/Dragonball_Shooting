#pragma once
#include <ddraw.h>
#include "Sprite.h"
#include "Timer.h"

class GameObject {
public:
	GameObject();
	~GameObject() = default;
	void Initialize(bool isLive, Sprite* pSprite, int x, int y, int curFrame, int frameInterval); 
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface, bool usingColor); 
	void DrawTargetFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface); 
	void Revive();
	void Kill();
	float GetX() const;
	float GetY() const;
	void SetXY(float x, float y);
	bool IsLive() const;

protected:
	float m_X;
	float m_Y;
	int m_FrameInteval; 
	int m_CurrentFrame; 
	bool m_IsLive;
	Sprite* m_Sprite;

private:
	system_clock::time_point m_LastFrameTime;
};

inline void GameObject::Revive()
{
	m_IsLive = true;
}

inline void GameObject::Kill()
{
	m_IsLive = false;
}

inline float GameObject::GetX() const
{
	return m_X;
}

inline float GameObject::GetY() const
{
	return m_Y;
}

inline void GameObject::SetXY(float x, float y)
{
	m_X = x;
	m_Y = y;
}

inline bool GameObject::IsLive() const
{
	return m_IsLive;
}

