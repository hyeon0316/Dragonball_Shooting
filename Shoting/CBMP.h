#ifndef __BMP_H__
#define __BMP_H__

#include <ddraw.h>
class CBMP
{
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char* m_pBuffer;

public:
	CBMP();
	~CBMP();
	int GetWidth();
	int GetHeight();
	bool LoadBMPFile(TCHAR* filename);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface);
};
#endif#pragma once
