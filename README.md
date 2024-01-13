# 2020 학교 개인 프로젝트 - DragonBall Shooting

## 프로젝트 소개
학교에서 제공받은 기본 2D 슈팅게임 소스를 활용하여 만든 프로젝트
- 개발기간 : 2020/06/10 ~ 2020/07/15
- 장르 : 횡스크롤 슈팅
- 그래픽 : 2D 
- 플랫폼 : PC
- 개발도구 : DirectX9 / WinAPI 

## 소개 영상
- https://youtu.be/k7A51_PhnA4

## 구현 및 시도 목록
- 상속 구조 및 virtual 키워드 사용하여 다형성 구현
- 오브젝트를 매번 생성하지 않고 풀링하여 재사용
- 전체적인 구조 수정 및 리팩토링

## 코드 목록
- 게임 로직
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Gamemain.cpp#L95-L414

- 오브젝트 상속 구조(플레이어, 적 등 다양한 오브젝트에 사용)
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/GameObject.cpp#L1-L46

- 플레이어 로직
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Player.cpp#L20-L200

- 적 로직
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Enemy.cpp#L7-L38
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Boss.cpp#L16-L148

- 적 스폰 관리
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/StageManager.cpp#L10-L73

- 애니메이션 재생(스프라이트를 순차적으로 보여줌)
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Animation.cpp#L3-L42

- 스프라이트 그리기
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Sprite.cpp#L97-L139
https://github.com/hyeon0316/Dragonball_Shooting/blob/695ccbda2dad9a6197e71f0747133dc08b1e7a34/Shoting/Sprite.cpp#L180-L220

- 시간 기록
https://github.com/hyeon0316/Dragonball_Shooting/blob/b0b77135ca91de62d73ddcd1cca11c6094bb92ca/Shoting/Timer.cpp#L5-L38
