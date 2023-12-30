#include <Windows.h>
#include "Sprite.h"

//����̹���
static RECT destRect; 
static RECT destRect2;

Sprite::Sprite() {
	destRect.left = 0;
	destRect.top = 0;
	destRect.right = 1366;
	destRect.bottom = 768;

	destRect2.left = -1366;
	destRect2.top = 0;
	destRect2.right = 0;
	destRect2.bottom = 768;

	m_ppSurface = NULL;
	m_nFrame = 0;
	m_pBMPArray = NULL;
}

Sprite::~Sprite() {
	if (m_ppSurface)
		delete[] m_ppSurface;

	if (m_pBMPArray)
		delete[] m_pBMPArray;
}

bool Sprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw) {
	if (m_ppSurface)
		delete[] m_ppSurface;

	if (m_pBMPArray)
		delete[] m_pBMPArray;

	m_ppSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_ppSurface)
		return false;

	m_pBMPArray = new CBMP[nFrame];

	if (!m_pBMPArray)
		return false;

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = nHeight;
	ddsd.dwWidth = nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = nColorKey;

	//DDCOLOCKEY ����ü
	// ���� : typedef struct _DDCOLORKEY { DWORD dwColorSpaceLowValue; DWORD dwColorSpaceHightValue; } DDCOLORKEY, FAR* LPDDCOLORKEY;
	// dwColorSpaceLowValue�� dwColorSpaceHighValue�� ���Ե� ������ ������ ��Ʈ������ ���ؼ� ȭ�鿡 ������ �ʴ´�.
	// ���� ��� 0���� 3������ ������ �����ϰ� �����Ϸ��� dwColorSpaceLowValue = 0, dwColorSpaceHighValue = 3���� �ϸ� �ȴ�.
	// �׷��� �̷� ������ �÷������� 8��Ʈ �ε��� ��忡���� ���ϸ�, 16,24,32��Ʈ������ ������ �����Ƿ� ���� ���� ����ϴ� ���� ����. (�� ���� ������)
	//**** �÷�Ű(COLORKEY) : �̹������� �����ϰ� ó���ؼ� ������ �ʴ� ��, ����Ű Ȥ�� ����Ű��� �θ�.
	for (int i = 0; i < nFrame; i++) {
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_ppSurface[i], NULL))) {
			m_ppSurface[i] = NULL;
			return false;
		}
		else
		{
			m_ppSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;

	return true;
}

bool Sprite::LoadFrame(int nFrame, TCHAR* filename)
{
	if (!m_pBMPArray[nFrame].LoadBMPFile(filename))
		return false;
	if (!m_pBMPArray[nFrame].CopyBufferToSurface(m_ppSurface[nFrame]))
		return false;

	return true;
}

bool Sprite::Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;

	destRect.left = x - (m_nWidth >> 1);
	destRect.top = y - (m_nHeight >> 1);
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	// Blt() : Ư�� ������ ���ǽ��� ��Ʈ�ϴ� �Լ� **��Ʈ: ���ǽ� ���� ��Ӻ��� ****�ø�(Flip) : ��ǥ��� ����ǥ���� �ּҸ� ���� �ٲ�
		// ù��° ���ڰ� lpDestRect�� ���� ���ǽ��� ����κп� �׷��� ������ ������ ������. NULL ���̸� �������ǽ� ��ü ������ �׷����� �ȴ�.
		// �ι�° ���ڰ� lpDDSrcSurf�� ������ ���� ���ǽ��̴�.
		// ����° ���ڰ� lpSrcRect�� ���� ���ǽ����� ������ ������ �����Ѵ�. NULL ���� �ָ� ���� ���ǽ� ��ü ������ �ȴ�.
		// �׹�° ���ڰ� dwFlags�� ��Ʈ�� �� �ʿ��� �÷��׵��� �����ϸ�, ����ϴ� �÷��׵��� ������ ����.
		// DDBLT_COLORFILL : BBLTFX ����ü�� dwFillColor����� �����Ͽ� ������ �簢�� ������ ��ĥ��.
		// DDBLT_DDFX : DDBLTFX ����ü�� dwDDFX����� ������ ȿ���� ���.
		// DDBLT_DDROPS : DDBLTFX ����ü�� dwDDROPS����� �����. Win32 API�� �������۾�(ROPs)�� �������.
		// DDBLT_KEYDEST : ������ ǥ�鿡 �ִ� ����Ű(ColorKey)�� �����.
		// DDBLT_KEYSRC : �ҽ�ǥ�鿡 �ִ� ����Ű(ColorKey)�� �����.
		// DDBLT_ROP : DDBLTFX ����ü�� dwROP ����� �̿��Ͽ� ROP �۾��� ��. Win32 API�� ���ǵ� �Ͱ� ����.
		// DDBLT_ROTATIONANGLE : DDBLTFX ����ü�� dwRotationAngle ����� ���ǽ��� ȸ�������� �����. 1/100�� �����̴�.
		// DDBLT_WAIT : ��Ʈ �۾��� ������ ������ ��� �����Ѵ�. �� �÷��״� �� ����� �Ѵ�
		// �ټ���° ���ڰ� lpDDBltFx�� DDBLTFX ����ü�� �������̸�, �� ����ü ������ ȸ��, ��Ī ���� �۾��� �� �� �ִ�.
		// �۾��� �����ϸ� DD_OK�� �����ϸ�, �����ϸ� �������� ������.
		// destRect�� ���ǽ��� m_ppSurface[nFrame] ��ü�� �����Ѵ�. ��Ʈ �۾��� ������ ������ ��� �����ϸ�, �ҽ�ǥ�鿡 �ִ� ����Ű�� ����Ѵ�.
	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
		return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
		return false;
	}
	return true;
}

bool Sprite::Restore()
{
	if (!m_ppSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (!m_ppSurface[i])
			return false;

		if (FAILED(m_ppSurface[i]->Restore()))
			return false;

		if (!m_pBMPArray[i].CopyBufferToSurface(m_ppSurface[i]))
			return false;
	}
	return true;
}

int Sprite::GetNumberOfFrame()
{
	return m_nFrame;
}

bool Sprite::ReleaseAll()
{
	if (!m_ppSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (m_ppSurface[i])
			m_ppSurface[i]->Release();
	}

	return true;

}

bool Sprite::Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) //��� ���ѽ�ũ��
{
	if (destRect.left >= 1366)
	{
		destRect.right = 1366;
		destRect.left = 0;

		destRect2.right = 0;
		destRect2.left = -1366;
	}

	destRect.right+=5;
	destRect.left += 5;

	destRect2.right += 5;
	destRect2.left += 5;

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return(Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect2, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	return true;
}