#ifndef __animation_h__
#define __animation_h__

#include "CGObject.h"

class CAnimation : public CGObject
{


public:
	CAnimation();
	~CAnimation();

	bool colorKey;
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void Drawskill(LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawEnding(LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif
