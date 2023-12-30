#pragma once
#include "GameObject.h"

class CAnimation : public GameObject
{
public:
	CAnimation()  = default;
	~CAnimation() = default;
	bool IsDrawEnd(LPDIRECTDRAWSURFACE7 lpSurface);
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawFrame(int frame, LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawEnding(LPDIRECTDRAWSURFACE7 lpSurface);
};
