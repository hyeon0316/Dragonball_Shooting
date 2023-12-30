#include "CAnimation.h" 

void CAnimation::DrawFrame(int frame, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_IsLive)
		return;

	GameObject::DrawTargetFrame(m_X, m_Y, frame, lpSurface);
}

void CAnimation::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_IsLive)
		return;

	if (m_CurrentFrame == m_Sprite->GetNumberOfFrame() - 1)
	{
		Kill();
	}
	GameObject::Draw(m_X, m_Y, lpSurface, true);
}

bool CAnimation::IsDrawEnd(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_IsLive)
		return false;

	if (m_CurrentFrame == m_Sprite->GetNumberOfFrame() - 1)
	{
		Kill();
		return true;
	}
	GameObject::Draw(m_X, m_Y, lpSurface, true);
	return false;
}

void CAnimation::DrawEnding(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(m_X, m_Y, lpSurface, false);
	if (m_CurrentFrame >= 206)
		m_CurrentFrame = 206;
}