#include <windows.h>
#include <windowsx.h>
#include "CBMP.h"

CBMP::CBMP()
{
	m_pBuffer = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
}

CBMP::~CBMP()
{
	if (m_pBuffer)
		delete[] m_pBuffer;
}

int CBMP::GetHeight()
{
	return (m_nHeight);
}

int CBMP::GetWidth()
{
	return (m_nWidth);
}

bool CBMP::LoadBMPFile(TCHAR* filename)
{
	HANDLE hfile;
	DWORD actualRead;

	hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	//CreateFile을 파일이 아닌 다른 장치에 대해 사용하는 경우 dwCreationDisposition 값을 반드시 OPEN_EXISTING 을 전달해야 한다.

	if (hfile == INVALID_HANDLE_VALUE)
		return false;

	BITMAPFILEHEADER bmpfilehead;

	/* typedef struct tagBITMAPFILEHEADER {
		WORD bfType;         // 비트맵의 종류, BMP는 0x4D42(BM)      2 바이트
		DWORD bfSize;        // 파일의 전체 크기, 바이트 단위        4
		WORD bfReserved1;	 // 항상 0
		WORD bfReserved2;	 // 항상 0
		DWORD bfOffBits;     // BITMAPINFO까지 바이트 수
	} BITMAPFILEHEADER */

	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL))
	{
		CloseHandle(hfile);
		return false;
	}

	if (bmpfilehead.bfType != 0x4D42)
	{
		CloseHandle(hfile);
		return false;
	}

	BITMAPINFOHEADER bmpinfohead;

	/*	typedef struct tagBITMAPINFOHEADER{
		DWORD      biSize;		// 이 구조체의 바이트 크기
		LONG       biWidth;		// 비트맵의 폭
		LONG       biHeight;		// 비트맵의 높이
		WORD       biPlanes;		// 색상 면의 수(항상 1)
		WORD       biBitCount;	// 픽셀당 비트수(1, 4, 8, 16, 24, 32)
		DWORD      biCompression;	// 압축종류, 압축되지 않은 비트맵 BI, RGB
		DWORD      biSizeImage;		// 이미지의 바이트 크기
		LONG       biXPelsPerMeter;	// 수평 해상도(X축)
		LONG       biYPelsPerMeter;	// 수직 해상도(Y축)
		DWORD      biClrUsed;			// 사용된 색상수
		DWORD      biClrImportant;	// 중요 색상의 수, 윈도우 애플리케이션용
	} BITMAPINFOHEADER */

	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))
	{
		CloseHandle(hfile);
		return false;
	}

	if (bmpinfohead.biBitCount != 24)
	{
		CloseHandle(hfile);
		return false;
	}

	int nHeight = bmpinfohead.biHeight;
	bool bBottomUp;

	if (nHeight > 0)
	{
		bBottomUp = true;
	}
	else
	{
		bBottomUp = false;
		nHeight = -nHeight;
	}

	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;

	if (m_pBuffer)
		delete[] m_pBuffer;
	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4];

	struct RGBstruct
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	RGBstruct *rgb24 = new RGBstruct[m_nWidth];
	int nStoredLine = (m_nWidth * 3 + 3) & ~3;
	int nRemainder = nStoredLine - (m_nWidth * 3);		//int nRemainder = 4 + (m_nWidth%4) - 4;
	unsigned char temp[4];
	int nDestY, nDeltaY;

	if (bBottomUp)
	{
		nDestY = nHeight - 1;
		nDeltaY = -1;
	}
	else
	{
		nDestY = 0;
		nDeltaY = 1;
	}

	for (int y = 0; y < nHeight; y++)
	{
		if (!ReadFile(hfile, rgb24, 3 * m_nWidth, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete[] m_pBuffer;
			return false;
		}
		for (int x = 0; x < m_nWidth; x++)
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), rgb24 + x, 3);
			*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0;
		}
		nDestY += nDeltaY;

		if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete[] m_pBuffer;
			return false;
		}
	}
	CloseHandle(hfile);

	return true;
}

bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_pBuffer)
		return false;

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));  //memset(&ddsd, 0, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//주표면을 그리기 위해서 Lock을 해줘야함, NULL은 핸들 이벤트 사용 안하겠다는 뜻
		return false;

	unsigned char* pDest, * pSrc;

	pDest = (unsigned char*)ddsd.lpSurface;

	pSrc = m_pBuffer;

	for (int y = 0; y < m_nHeight; y++)
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);
		pDest += ddsd.lPitch;		// lPitch : 표면의 메모리피치로, 한 줄당 바이트 수를 말한다.
		pSrc += (m_nWidth << 2);
	}

	lpSurface->Unlock(NULL);

	return true;
}

