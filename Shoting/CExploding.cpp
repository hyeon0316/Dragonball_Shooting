#include "CExploding.h" 

CExploding::CExploding()
{
}

CExploding::~CExploding()
{
}

void CExploding::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface,true);
	if(m_blsLive)
		if (m_nCurrentFrame == m_pSprite->GetNumberOfFrame()-1) 
			Kill();
}
