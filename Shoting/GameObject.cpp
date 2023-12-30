#include "GameObject.h"

GameObject::GameObject()
	: m_X(0)
	, m_Y(0)
	, m_FrameInteval(0)
	, m_CurrentFrame(0)
	, m_LastFrameTime()
	, m_IsLive(false)
	, m_Sprite(nullptr)
{
}

void GameObject::Initialize(bool isLive, Sprite* pSprite, int x, int y, int curFrame, int frameInterval)
{
	m_Sprite = pSprite;
	m_CurrentFrame = curFrame;
	m_X = static_cast<float>(x);
	m_Y = static_cast<float>(y);
	m_FrameInteval = frameInterval;
	m_IsLive = isLive;
	m_LastFrameTime = Timer::Now();
}

void GameObject::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface, bool usingColor) {
	if (!m_IsLive)
		return;

	if (Timer::Elapsed(m_LastFrameTime, m_FrameInteval))
	{
		m_CurrentFrame = ++m_CurrentFrame % m_Sprite->GetNumberOfFrame(); //이미지를 차례로 보여줌
	}
	m_Sprite->Drawing(m_CurrentFrame, m_X, m_Y, lpSurface);
}


/// <summary>
/// //해당 프레임 번호의 이미지를 보여줌
/// </summary>
void GameObject::DrawTargetFrame(int x, int y, int frame, LPDIRECTDRAWSURFACE7 lpSurface) 
{
	if (!m_IsLive)
		return;

	m_Sprite->Drawing(frame, m_X, m_Y, lpSurface);
}