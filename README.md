# 학교 과제 - DragonBall Shooting

## 프로젝트 정보
- 기간 : 2020/06/10 ~ 2020/07/15 (2023/12 ~ 2024/01 객체 구조 리팩터링)
- 장르 : 횡스크롤 슈팅(2D)
- 플랫폼 : PC
- 도구 : C++, WinAPI, DirectX SDK(DirectDraw 7)
- 인원 : 1인

학교 수업에서 제공한 2D 슈팅 프레임워크를 바탕으로 횡스크롤 전환, 보스 3종, 아이템·스킬 게이지·배리어, 스테이지 전환, 컷신을 추가함.

## 코드
### BMP 로딩 최적화

컷신과 고해상도 스프라이트를 추가하면서 리소스가 3.2GB가 되었고, 게임 시작 시 모든 스프라이트를 미리 읽는 구조라 로딩이 크게 느려짐.

| 리소스 | 용량 |
|---|---|
| 전체 | 3.2 GB (전부 무압축 24bit BMP) |
| 보스 등장 컷신 | 1.6 GB |
| 엔딩 컷신 | 1.3 GB |
| 필살기 컷신 | 422 MB (71프레임 × 1920×1080) |

- 원인

BMP는 무압축 포맷이라 픽셀 1개가 파일의 3바이트이고, 프레임워크의 로더는 이 3바이트를 픽셀마다 `ReadFile()`로 읽고 있었음.
`ReadFile()`은 커널 모드 전환을 동반하는 시스템 콜이라 호출 1회의 비용이 복사하는 3바이트보다 훨씬 큼.
로딩 시간 대부분이 데이터 전송이 아니라 호출 오버헤드에 쓰임. (3.2GB / 3byte ≈ 약 10억 회 호출)
```cpp
for (int y = 0; y < nHeight; y++)
{
    for (int x = 0; x < m_nWidth; x++)
    {
        ReadFile(hfile, &rgb24, 3, &actualRead, NULL); //픽셀마다 시스템 콜
        ...
    }
    ReadFile(hfile, temp, nRemainder, &actualRead, NULL); //줄 끝 패딩
}
```

- 해결

BMP는 픽셀이 행 단위로 연속 배치되므로, 읽기 단위를 가로 한 줄로 바꿔 한 줄 전체를 `ReadFile()` 1회로 받음.

한 줄 크기의 버퍼 (`nStoredLine`은 BMP의 4바이트 행 정렬 규격, `nRemainder`는 줄 끝 패딩 크기)
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/CBMP.cpp#L116-L118

`ReadFile()`을 픽셀 루프 밖으로 옮기고 읽기 크기를 `3 * m_nWidth`로 지정
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/CBMP.cpp#L133-L154

- 결과

| | 픽셀 단위 | 스캔라인 단위 |
|---|---|---|
| 1920×1080 1프레임당 `ReadFile` 호출 (패딩 읽기 포함) | 2,074,680 회 | 2,160 회 |
| 감소 비율 | — | **약 1/960** |

- 개선점

| 항목 | 문제 | 개선 방향 |
|---|---|---|
| 줄 버퍼 미해제 | `new RGBstruct[m_nWidth]`를 해제하지 않아 파일마다 한 줄 크기만큼 누수 | 함수 끝에서 `delete[]`, 또는 `std::vector` 사용 |
| 패딩이 0바이트여도 호출 | 가로 바이트 수가 4의 배수면 패딩이 없는데도 `ReadFile()`을 호출 | `nRemainder > 0`일 때만 호출하거나, 패딩까지 포함한 `nStoredLine` 크기로 한 번에 읽음 |

<br></br>

### 상속 구조 (2023/12 ~ 2024/01 리팩터링)

모든 적을 `vector<Enemy*>` 하나로 관리하고, 계층별로 책임을 나눔.
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

`ExecutePattern()`과 `TakeDamage()`를 순수 가상으로 선언해, 적을 추가하면 두 함수를 반드시 구현해야 함.
구현을 빠뜨리면 컴파일 단계에서 잡힘.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Enemy.h#L5-L22

**2. 메인 루프에 타입 분기 없음**

메인 루프는 구체 타입을 모른 채 같은 호출만 함. 적 종류가 늘어도 이 루프는 수정하지 않음.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Gamemain.cpp#L139-L143

**3. 사망 시 부수 효과를 `Kill()` 오버라이드로 각 클래스가 소유**

"죽었을 때 무슨 일이 일어나는가"를 각 클래스 안에 두고, 공통 처리(폭발 이펙트)는 부모를 호출해 재사용함.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Enemy.cpp#L14-L18

**4. 패턴 함수를 감추고 진입점 단일화**

`Boss`는 패턴 3종을 `private`으로 두고 `SetPattern(order)` 하나만 `protected`로 공개함.
하위 보스는 자기 패턴 번호만 지정하므로 `ExecutePattern()` 구현이 한 줄로 끝남.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.h#L4-L21

| 항목 | 구조 |
|---|---|
| 적 관리 | `vector<Enemy*>` 단일 컨테이너 |
| 메인 루프 | 적마다 가상 함수 호출 |
| 적 추가 시 | 클래스 1개 작성 + `push_back` |
| 사망 처리 | 각 클래스의 `Kill()` 오버라이드 |

<br></br>

### 보스 패턴

**패턴 1 — 상하 이동 + 확산탄**

화면 오른쪽으로 등장한 뒤 위아래로 왕복하며, 일정 간격으로 세 방향 탄을 발사함.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L23-L66

**패턴 2 — 순간이동 + 광역 빔**

2초마다 정해진 높이 3곳 중 하나로 순간이동하고, 1초 예고 후 가로 빔을 발사함.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L68-L106

**패턴 3 — 무작위 낙하탄**

화면 위의 무작위 x 위치에서 탄이 떨어짐.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Boss.cpp#L108-L127

<br></br>

### 시간 처리

`Timer`를 정적 클래스로 두고, 각 객체가 마지막 실행 시각을 `time_point`로 따로 가짐.
`Elapsed()`가 경과 판정과 기준 시각 갱신을 함께 처리해, 호출부는 쿨타임을 한 줄로 표현함.
https://github.com/hyeon0316/Dragonball_Shooting/blob/58a066583ca22275013741cb768e897bf400888f/Shoting/Timer.cpp#L27-L38

```cpp
// 플레이어 연사 간격 240ms
bool Player::CanAttack()
{
    if (!m_IsLive) return false;
    return Timer::Elapsed(m_LastAttackTime, MY_MISSILE_INTRVAL);
}
```

- 개선점

| 항목 | 문제 | 개선 방향 |
|---|---|---|
| `system_clock` 사용 | 시스템 시각 변경·NTP 동기화 시 값이 뒤로 갈 수 있어 경과 시간 측정에 부적합 | 단조 증가가 보장되는 `steady_clock`으로 교체 |
