#include <Windows.h>
#include "Sprite.h"

//배경이미지
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

	//DDCOLOCKEY 구조체
	// 원형 : typedef struct _DDCOLORKEY { DWORD dwColorSpaceLowValue; DWORD dwColorSpaceHightValue; } DDCOLORKEY, FAR* LPDDCOLORKEY;
	// dwColorSpaceLowValue와 dwColorSpaceHighValue에 대입된 범위의 색상은 비트연산을 통해서 화면에 나오지 않는다.
	// 예를 들어 0부터 3까지의 값들을 투명하게 설정하려면 dwColorSpaceLowValue = 0, dwColorSpaceHighValue = 3으로 하면 된다.
	// 그러나 이런 범위의 컬러값들은 8비트 인덱스 모드에서만 통하며, 16,24,32비트에서는 통하지 않으므로 같은 값을 사용하는 것이 좋다. (두 값이 같도록)
	//**** 컬러키(COLORKEY) : 이미지에서 투명하게 처리해서 나오지 않는 색, 색상키 혹은 투명키라고도 부름.
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

	// Blt() : 특정 영역에 서피스를 블리트하는 함수 **블리트: 서피스 간의 고속복사 ****플립(Flip) : 주표면과 보조표면의 주소를 서로 바꿈
		// 첫번째 인자값 lpDestRect는 목적 서피스의 어느부분에 그려줄 것인지 영역을 지정함. NULL 값이면 목적서피스 전체 영역에 그려지게 된다.
		// 두번째 인자값 lpDDSrcSurf는 복사할 원본 서피스이다.
		// 세번째 인자값 lpSrcRect는 원본 서피스에서 복사할 영역을 지정한다. NULL 값을 주면 원본 서피스 전체 영역이 된다.
		// 네번째 인자값 dwFlags는 블리트할 때 필요한 플래그들을 지정하며, 사용하는 플래그들은 다음과 같다.
		// DDBLT_COLORFILL : BBLTFX 구조체의 dwFillColor멤버를 참조하여 지정된 사각형 영역을 색칠함.
		// DDBLT_DDFX : DDBLTFX 구조체의 dwDDFX멤버에 설정한 효과를 사용.
		// DDBLT_DDROPS : DDBLTFX 구조체의 dwDDROPS멤버를 사용함. Win32 API의 래스터작업(ROPs)와 상관없음.
		// DDBLT_KEYDEST : 목적지 표면에 있는 색상키(ColorKey)를 사용함.
		// DDBLT_KEYSRC : 소스표면에 있는 색상키(ColorKey)를 사용함.
		// DDBLT_ROP : DDBLTFX 구조체의 dwROP 멤버를 이용하여 ROP 작업을 함. Win32 API에 정의된 것과 같음.
		// DDBLT_ROTATIONANGLE : DDBLTFX 구조체의 dwRotationAngle 멤버를 서피스의 회전각도로 사용함. 1/100도 단위이다.
		// DDBLT_WAIT : 블리트 작업이 성공할 때까지 계속 수행한다. 이 플래그는 꼭 써줘야 한다
		// 다섯번째 인자값 lpDDBltFx는 DDBLTFX 구조체의 포인터이며, 이 구조체 값으로 회전, 대칭 등의 작업을 할 수 있다.
		// 작업에 성공하면 DD_OK를 리턴하며, 실패하면 에러값을 돌려줌.
		// destRect에 서피스인 m_ppSurface[nFrame] 전체를 복사한다. 블리트 작업이 성공할 때까지 계속 수행하며, 소스표면에 있는 색상키를 사용한다.
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

bool Sprite::Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) //배경 무한스크롤
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