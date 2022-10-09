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
	//CreateFile�� ������ �ƴ� �ٸ� ��ġ�� ���� ����ϴ� ��� dwCreationDisposition ���� �ݵ�� OPEN_EXISTING �� �����ؾ� �Ѵ�.

	if (hfile == INVALID_HANDLE_VALUE)
		return false;

	BITMAPFILEHEADER bmpfilehead;

	/* typedef struct tagBITMAPFILEHEADER {
		WORD bfType;         // ��Ʈ���� ����, BMP�� 0x4D42(BM)      2 ����Ʈ
		DWORD bfSize;        // ������ ��ü ũ��, ����Ʈ ����        4
		WORD bfReserved1;	 // �׻� 0
		WORD bfReserved2;	 // �׻� 0
		DWORD bfOffBits;     // BITMAPINFO���� ����Ʈ ��
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
		DWORD      biSize;		// �� ����ü�� ����Ʈ ũ��
		LONG       biWidth;		// ��Ʈ���� ��
		LONG       biHeight;		// ��Ʈ���� ����
		WORD       biPlanes;		// ���� ���� ��(�׻� 1)
		WORD       biBitCount;	// �ȼ��� ��Ʈ��(1, 4, 8, 16, 24, 32)
		DWORD      biCompression;	// ��������, ������� ���� ��Ʈ�� BI, RGB
		DWORD      biSizeImage;		// �̹����� ����Ʈ ũ��
		LONG       biXPelsPerMeter;	// ���� �ػ�(X��)
		LONG       biYPelsPerMeter;	// ���� �ػ�(Y��)
		DWORD      biClrUsed;			// ���� �����
		DWORD      biClrImportant;	// �߿� ������ ��, ������ ���ø����̼ǿ�
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

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//��ǥ���� �׸��� ���ؼ� Lock�� �������, NULL�� �ڵ� �̺�Ʈ ��� ���ϰڴٴ� ��
		return false;

	unsigned char* pDest, * pSrc;

	pDest = (unsigned char*)ddsd.lpSurface;

	pSrc = m_pBuffer;

	for (int y = 0; y < m_nHeight; y++)
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);
		pDest += ddsd.lPitch;		// lPitch : ǥ���� �޸���ġ��, �� �ٴ� ����Ʈ ���� ���Ѵ�.
		pSrc += (m_nWidth << 2);
	}

	lpSurface->Unlock(NULL);

	return true;
}

