#ifndef __sprite_h__
#define __sprite_h__

#include <ddraw.h>

#include "CBMP.h" 

class CSprite
{
private:
	int m_nWidth;
	int m_nHeight;
	int m_nFrame;
	LPDIRECTDRAWSURFACE7* m_ppSurface;

	CBMP* m_pBMPArray;
public:
	CSprite();
	~CSprite();
	int GetNumberOfFrame();
	bool InitSprite(int nFrame, int nWidth, int Height, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int nFrame, TCHAR* filename);
	bool Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Restore();
	bool ReleaseAll();
};
#endif#pragma once
