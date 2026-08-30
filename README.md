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

원본 리소스는 6.4MB였으나, 컷신과 고해상도 스프라이트를 추가하며 3.2GB(약 500배)가 됨.
원본 규모에서는 드러나지 않던 로더의 I/O 방식이 병목으로 표면화되어, 게임 시작 전 로딩 속도가 많이 느린 상태였음.

| 리소스 | 용량 |
|---|---|
| 전체 | 3.2 GB (전부 무압축 24bit BMP) |
| 보스 등장 컷신 | 1.6 GB |
| 엔딩 컷신 | 1.3 GB |
| 필살기 컷신 | 422 MB (71프레임 × 1920×1080) |

- 원본

BMP는 무압축 포맷이라 픽셀 1개가 곧 파일의 3바이트임.
기존 코드는 이 3바이트를 픽셀마다 `ReadFile()`로 개별 호출하고 있었음.

`ReadFile()`은 유저 모드 ↔ 커널 모드 전환을 동반하는 시스템 콜이라, 호출 1회당 오버헤드가 실제 복사하는 3바이트보다 압도적으로 큼.
즉 로딩 시간의 대부분이 데이터 전송이 아니라 **호출 오버헤드**에 쓰이고 있었음. (1920×1080 컷신 1프레임 기준 약 207만 회 호출)
```
	RGBstruct rgb24; //픽셀 1개짜리 버퍼

	for (int y = 0; y < nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (!ReadFile(hfile, &rgb24, 3, &actualRead, NULL)) //픽셀마다 시스템 콜
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

파일 I/O 단위를 픽셀 → 스캔라인(가로 한 줄)으로 변경.
BMP는 픽셀이 행 단위로 연속 배치되므로, 한 줄 전체를 `ReadFile()` 1회로 받을 수 있음.

픽셀 1개 -> 한줄 크기의 버퍼로 수정 (`nStoredLine`은 BMP의 4바이트 행 정렬 규격, `nRemainder`는 줄 끝 패딩 크기)
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/CBMP.cpp#L116-L118

`ReadFile()`을 내부 픽셀 루프 밖으로 이동, 읽기 크기를 `3` -> `3 * m_nWidth`로 변경
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/CBMP.cpp#L133-L154

남아있는 픽셀 루프는 디스크 접근이 아니라 **24bit BGR → DirectDraw 서피스용 32bit BGRA 변환**임.
비싼 쪽(시스템 콜)만 줄이고, 메모리 내 변환은 그대로 둠.

- 결과

| | 원본 (픽셀 단위) | 수정 (스캔라인 단위) |
|---|---|---|
| 1920×1080 1프레임당 `ReadFile` 호출 | 2,073,600 회 | 1,080 회 |
| 감소 비율 | — | **약 1/1920** |

로딩 화면이 넘어가는 속도가 눈에 띄게 개선됨.


