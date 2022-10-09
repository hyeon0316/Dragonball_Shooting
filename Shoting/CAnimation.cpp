#include "CAnimation.h" 
CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface, false);
	if (m_blsLive)
		if (m_nCurrentFrame == m_pSprite->GetNumberOfFrame() - 1)												
			Kill();
}
void CAnimation::Drawskill(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface, true);
	if (m_blsLive)
		if (m_nCurrentFrame == m_pSprite->GetNumberOfFrame() - 1)
			Kill();
}

void CAnimation::DrawEnding(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface, false);
	if (m_blsLive)
		if (m_nCurrentFrame >= 206)
			m_nCurrentFrame = 206;
}