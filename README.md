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


- 결과

| | 원본 (픽셀 단위) | 수정 (스캔라인 단위) |
|---|---|---|
| 1920×1080 1프레임당 `ReadFile` 호출 | 2,073,600 회 | 1,080 회 |
| 감소 비율 | — | **약 1/1920** |

로딩 화면이 넘어가는 속도가 눈에 띄게 개선됨.

### 상속 구조 설계

원본은 적이 잡몹 1종 + 보스 1종(좌우 이동만)이었고, 메인 루프에서 타입별로 직접 분기하는 구조였음.
보스 3체와 패턴 3종을 추가하면서 구조를 다시 잡음.

- 원본

적 종류마다 전용 배열을 두고, 메인 루프가 각각을 따로 순회하며 이동/충돌/그리기를 처리.
```
CGObject
├── CMyShip
├── CEnemy / CEnemyMissile
├── CBoss  / CBossMissile
├── CMyMissile
└── CExploding
```
```cpp
// 원본 gamemain.cpp — 타입마다 별도 루프, 충돌 판정은 4중 중첩
for (m = 0; m < MAX_MISSILES; m++)
    for (i = 0; i < MAX_XENEMYS; i++)
        for (j = 0; j < MAX_YENEMYS; j++)
            if (g_MyMissile[m].IsLive() && g_Enemy[i][j].IsLive())
                if (abs(g_MyMissile[m].GetX() - g_Enemy[i][j].GetX()) < 30)
                    ...
```
적 종류를 추가하려면 배열 선언 / 초기화 / 이동 / 충돌 / 그리기 루프를 전부 새로 써야 하는 구조였음.

- 수정

계층별로 책임을 나누고, 모든 적을 `vector<Enemy*>` 하나로 관리.
```
GameObject                      // 위치, 생존 여부, 스프라이트, 프레임 애니메이션
├── Player                      // 입력, HP/MP, 필살기, 배리어
├── Enemy (추상)                // 피격/사망 처리, 폭발 이펙트
│   ├── NormalEnemy             // 지그재그 이동 + 랜덤 사격 + 아이템 드랍
│   └── Boss (추상)             // 패턴 3종 보유
│       ├── FirstBoss           // 상하 이동 + 확산탄
│       ├── SecondBoss          // 순간이동 + 광역 빔
│       └── LastBoss            // 무작위 낙하탄
├── PlayerMissile / EnemyMissile / BossMissile / LastBossMissile
├── Item / ItemSlot
└── Animation                   // 1회 재생 후 자동 소멸 이펙트
```

**1. 순수 가상 함수로 하위 클래스 구현 강제**

`ExecutePattern()`과 `TakeDamage()`를 순수 가상으로 선언해, 적을 추가하면 두 함수를 반드시 구현하게 만듦.
구현을 빠뜨리면 컴파일 단계에서 잡힘.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Enemy.h#L5-L22

**2. 메인 루프에서 타입 분기 제거**

메인 루프는 구체 타입을 모른 채 동일한 호출만 함. 적 종류가 늘어도 이 루프는 수정할 필요가 없음.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Gamemain.cpp#L139-L143

**3. `Kill()` 오버라이드로 사망 시 부수 효과를 각 클래스가 소유**

"죽었을 때 무슨 일이 일어나는가"를 각 클래스 안에 두고, 공통 처리(폭발 이펙트)는 부모를 호출해 재사용.
메인 루프에 `if (적 종류 == ...)` 분기가 전혀 없음.

공통 처리는 부모가 담당
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Enemy.cpp#L14-L18

**4. 패턴 함수를 감추고 진입점 단일화**

`Boss`는 패턴 3종을 `private`으로 두고 `SetPattern(order)` 하나만 `protected`로 공개.
하위 보스는 자기 패턴 번호만 지정하면 되므로 `ExecutePattern()` 구현이 한 줄로 끝남.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.h#L4-L21

- 결과

| | 원본 | 수정 |
|---|---|---|
| 적 관리 | 종류별 개별 배열 | `vector<Enemy*>` 단일 컨테이너 |
| 메인 루프 | 타입마다 별도 루프 | 가상 함수 호출 1회 |
| 적 추가 시 | 배열/초기화/이동/충돌/그리기 전부 추가 | 클래스 1개 작성 + `push_back` |
| 사망 처리 | 메인 루프에서 타입별 분기 | 각 클래스의 `Kill()` 오버라이드 |

### 시간 처리 교체

- 원본

`timeGetTime()` 반환값을 `int`로 받아 다룸.
```cpp
int CTimer::time() { return (timeGetTime() - m_nStartTime); }
bool CTimer::elapsed(int& stime, int interval) { ... }
```
시간 값이 일반 정수와 구분되지 않아, 단위(ms인지 프레임인지)가 코드에 드러나지 않고 다른 정수와 섞여도 컴파일러가 잡지 못함.

- 수정

`std::chrono::system_clock::time_point`로 교체해 타입 레벨에서 시간을 구분.
`Timer`를 정적 클래스로 만들어 각 객체가 타이머 포인터를 들고 다니지 않게 함(원본은 `CTimer*`를 생성자마다 전달).
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Timer.h#L7-L19

`Elapsed()`가 경과 판정과 기준 시각 갱신을 함께 처리해, 호출부는 쿨타임을 한 줄로 표현.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Timer.cpp#L27-L38

```cpp
// 플레이어 연사 간격 240ms
bool Player::CanAttack()
{
    if (!m_IsLive) return false;
    return Timer::Elapsed(m_LastAttackTime, MY_MISSILE_INTRVAL);
}
```

객체마다 `time_point`를 개별 보유해 서로 독립된 쿨타임을 가짐.
(플레이어 이동/공격/배리어, 보스 순간이동/스킬, 적 이동, 애니메이션 프레임 전환이 전부 별개 타이머)

### 보스 패턴 시스템

**패턴 1 — 상하 이동 + 확산탄**
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L23-L66

**패턴 2 — 순간이동 + 광역 빔**
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L68-L106

**패턴 3 — 무작위 낙하탄**
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L108-L127
