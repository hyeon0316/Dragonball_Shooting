#ifndef __initdirectx_H__
#define __initdirectx_H__


class init
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
#endif
