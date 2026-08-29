# DragonBall Shooting

## 프로젝트 정보
- 기간 : 2020/06/10 ~ 2020/07/15
- 장르 : 횡스크롤 슈팅(2D)
- 플랫폼 : PC
- 도구 : DirectX9, WinAPI, C++

## 영상 자료
- https://youtu.be/k7A51_PhnA4

## 코드 
### BMP 로딩 최적화
- 원본
```
	for (int y = 0; y < nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (!ReadFile(hfile, &rgb24, 3, &actualRead, NULL))
			{
				CloseHandle(hfile);
				delete[] m_pBuffer;
				return false;
			}

			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), &rgb24, 3);
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
```
- 수정
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/CBMP.cpp#L133-L154


