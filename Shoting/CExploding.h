#ifndef __exploding_h__
#define __exploding_h__

  #include "CGObject.h"

class CExploding : public CGObject
{
public:
	CExploding();
	~CExploding();
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif

