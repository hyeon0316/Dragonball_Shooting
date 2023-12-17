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

## 코드 목록
- 모든 객체가 상속받는 기본적인 클래스로 필요한 변수와 함수를 포함
https://github.com/hyeon0316/Dragonball_Shooting/blob/c2da5ed6916be5215312fc4454da974bb014062e/Shoting/CGObject.cpp#L3-L65
<br/>

- 스프라이트 그리기(순차적으로 표시(애니메이션), 배경 무한스크롤, 스킬 표현 등)
https://github.com/hyeon0316/Dragonball_Shooting/blob/0c62a358674f8912d1c1b49ad0bb49333aadea45/Shoting/CGObject.cpp#L12-L58
https://github.com/hyeon0316/Dragonball_Shooting/blob/0c62a358674f8912d1c1b49ad0bb49333aadea45/Shoting/CSprite.cpp#L32-L222

- 경과된 시간 값을 통해 플레이어, 적, 발사체 움직임 등 여러 동작에 사용되는 시간 측정
https://github.com/hyeon0316/Dragonball_Shooting/blob/c2da5ed6916be5215312fc4454da974bb014062e/Shoting/CTime.cpp#L14-L34
<br/>

- 스폰되는 적과 플레이어의 기본적인 스탯과 피격 판정, 패턴 결정 등 전체적인 게임진행 관리
https://github.com/hyeon0316/Dragonball_Shooting/blob/c2da5ed6916be5215312fc4454da974bb014062e/Shoting/gamemain.cpp#L1210-L1307
https://github.com/hyeon0316/Dragonball_Shooting/blob/c2da5ed6916be5215312fc4454da974bb014062e/Shoting/gamemain.cpp#L1309-L1349
<br/>

- 보스의 발사체, 움직임에 대한 패턴 정의
https://github.com/hyeon0316/Dragonball_Shooting/blob/c2da5ed6916be5215312fc4454da974bb014062e/Shoting/CBoss.cpp#L33-L118


