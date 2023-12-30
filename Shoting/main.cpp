#define WIN_LEAN_AND_MEAN	  //윈도우 헤더 중복하지 않겠다

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dsound.lib")

#include<windows.h>		//기본 윈도우 헤더파일
#include<windowsx.h>	//많은 매크로가 정의 되어있다.
#include<ddraw.h>
#include<dinput.h>

#include"CBMP.h"
#include"Timer.h"
#include"Sprite.h"
#include"Enemy.h"
#include"Player.h"
#include"PlayerMissile.h"
#include"EnemyMissile.h"
#include"BossMissile.h"
#include "Item.h"
#include "CAnimation.h"
#include "ItemSlot.h"
#include "GameEnum.h"
#include "FirstBoss.h"
#include "SecondBoss.h"
#include "LastBoss.h"
#include "define.h"


LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern bool InitializeDirectX(void);	
extern BOOL _InitDirectSound();

void IntroMain();
void GameMain();
void InitGame();
bool LoadBMPandInitSurface();

HWND g_hwnd;
HINSTANCE g_hInstance;
// LPDIRECTDRAW - DirectDraw, 
// 화면제어를 전담하는 객체 LPDIRECTDRAW 클래스로 선언
// 그래픽카드를 제어
// 관련 Library :  ddraw.lib

// 비디오 카드를 나타낸다. 이인터페이스를 사용하여 비디오모드를 선택하고,
// 시스템의 전체적인 협력 수준을 정한다. IDirectDraw7은 생성하는 COM 객체의 주요 인터페이스(핵심)이며
// 이로부터 다른 인터페이스들을 요청한다.

LPDIRECTDRAW7 g_IpDirectDrawObject = NULL;
LPDIRECTDRAWSURFACE7 g_IpPrimarySurface = NULL;
LPDIRECTDRAWSURFACE7 g_IpSecondarySurface = NULL;
LPDIRECTINPUT8 g_IpDirectInputObject = NULL;
LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard = NULL;

Sprite g_Boss3MissileSprite;
Sprite g_EndingSprite;
Sprite g_Boss3Sprite;
Sprite g_Boss3IntroSprite;
Sprite g_Boss2MissileSprite;
Sprite g_Boss2Sprite;
Sprite g_MaxPowerSprite;
Sprite g_SkillLoadingBarSprite;
Sprite g_SkillSprite;
Sprite g_SkillSceneSprite;
Sprite g_ItemSlotSprite;
Sprite g_BossIconSprite;
Sprite g_BarrierEffectSprite;
Sprite g_ItemSprite;
Sprite g_LifeSprite;
Sprite g_PlayerIconSprite;
Sprite g_GameStartSprite;
Sprite g_QuitSprite;
Sprite g_IntroSprite;
Sprite g_TitleSprite;
Sprite g_BackgroundSprite;
Sprite g_EnemySprite;
Sprite g_EnemyMissileSprite;
Sprite g_MyShipSprite;
Sprite playerMissileSprite;
Sprite g_ExplodingSprite;
Sprite g_Boss1Sprite;
Sprite g_Boss1MissileSprite;
Sprite g_ClearSprite;
Sprite g_DeadSprite;
Sprite g_LoadingSprite;

EMod curMod;

bool isActiveApp = false;

/*
1. DirectDraw 객체 생성
2. 윈도우 협력 수준 설정
3. 디스플레이 모드 선택
4. Primary Surface 만들기( 주 표면 )
5. Secondary Surface 만들기( 보조 표면 )
6. 주표면과 보조표면 플립(Flip)하기
7. 256 컬러를 표현하기 위해 팔레트 사용
8. Blt(), BltFast() 블리터를 이용하여 그림 그리기
9. 컬러키(ColorKey)와 투명도 설정
10. 윈도우 창모드 사용시 클리퍼(Clipping) 사용하기
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow) { //시작지점
	
	WNDCLASSEX wndclass; //윈도우 속성 정의
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc; //메시지 발생할 경우 WndProc으로 전달
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = (LPCSTR)CLASS_NAME;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wndclass) == 0)
		return 0;
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST, (LPCSTR)CLASS_NAME, (LPCSTR)CLASS_NAME, WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	ShowWindow(g_hwnd, nCmdShow); //윈도우 화면에 표시

	if (g_hwnd == NULL)return 0;
	g_hInstance = hInstance;

	SetFocus(g_hwnd);
	ShowCursor(FALSE);
	if (!InitializeDirectX())
		return 0;
	if (!_InitDirectSound())
		return 0;
	Timer::Start();
	if (!LoadBMPandInitSurface())
		return 0;
	curMod = EMod::Intro;

	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) { //메시지가 없을때도 즉각 리턴
			if (!GetMessage(&msg, NULL, 0, 0)) //사용자로 부터 메시지 처리
				return(int)msg.wParam;
			TranslateMessage(&msg); //입력 메시지를 가공하여 프로그램에서 사용 가능하도록 함
			DispatchMessage(&msg); // 윈도우 메시지 처리함수인 WndProc으로 전달
		}
		else if (isActiveApp)
		{
			Timer::UpdateDeltatime();
			switch (curMod)
			{
			case EMod::Intro:
				IntroMain();
				break;
			case EMod::Game:
				GameMain();
				break;
			case EMod::Quit:
				break;
			default:
				break;
			}
		}
		else 
		{
			WaitMessage();
		}
	}
}


LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { //실질적인 프로그램 처리
	
	switch (message) {
	case WM_ACTIVATEAPP:
		if (wParam)
			isActiveApp = true;
		else
			isActiveApp = false;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (g_IpDirectDrawObject) {
			if (g_IpSecondarySurface)
				g_IpSecondarySurface->Release();
			if (g_IpPrimarySurface)
				g_IpPrimarySurface->Release();
			g_DeadSprite.ReleaseAll();
			g_ClearSprite.ReleaseAll();
			g_Boss1MissileSprite.ReleaseAll();
			g_Boss1Sprite.ReleaseAll();
			g_EnemyMissileSprite.ReleaseAll();
			g_EnemySprite.ReleaseAll();
			g_BackgroundSprite.ReleaseAll();
			g_TitleSprite.ReleaseAll();
			g_LoadingSprite.ReleaseAll();
			g_IntroSprite.ReleaseAll();
			g_GameStartSprite.ReleaseAll();
			g_QuitSprite.ReleaseAll();
			g_PlayerIconSprite.ReleaseAll();
			g_LifeSprite.ReleaseAll();
			g_ItemSprite.ReleaseAll();
			g_BarrierEffectSprite.ReleaseAll();
			g_BossIconSprite.ReleaseAll();
			g_ItemSlotSprite.ReleaseAll();
			g_SkillSceneSprite.ReleaseAll();
			g_SkillSprite.ReleaseAll();
			g_SkillLoadingBarSprite.ReleaseAll();
			g_MaxPowerSprite.ReleaseAll();
			g_Boss2Sprite.ReleaseAll();
			g_Boss2MissileSprite.ReleaseAll();
			g_Boss3IntroSprite.ReleaseAll();
			g_Boss3Sprite.ReleaseAll();
			g_EndingSprite.ReleaseAll();
			g_Boss3MissileSprite.ReleaseAll();

			g_IpDirectDrawObject->Release();
		}
		if (g_IpDirectInputObject)
		{
			if (g_IpDirectInputKeyboard)
			{
				g_IpDirectInputKeyboard->Unacquire();
				g_IpDirectInputKeyboard->Release();
			}
			g_IpDirectInputObject->Release();
		}
		ShowCursor(TRUE);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam); // WinProc에서 처리하지 않은 나머지 메시지를 윈도우 기본으로 처리하도록 전달
	}
	return 0;
}


bool LoadBMPandInitSurface() {
	
	if (!g_TitleSprite.InitSprite(1, 750, 421, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_TitleSprite.LoadFrame(0, TEXT("data\\Loading_image.bmp")))
		return false;
	g_TitleSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, g_IpSecondarySurface, false);

	if (!g_LoadingSprite.InitSprite(11, 400, 40, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_LoadingSprite.LoadFrame(0, TEXT("data\\Roading1.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(1, TEXT("data\\Roading2.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(2, TEXT("data\\Roading3.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(3, TEXT("data\\Roading4.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(4, TEXT("data\\Roading5.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(5, TEXT("data\\Roading6.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(6, TEXT("data\\Roading7.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(7, TEXT("data\\Roading8.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(8, TEXT("data\\Roading9.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(9, TEXT("data\\Roading10.bmp")))
		return false;
	if (!g_LoadingSprite.LoadFrame(10, TEXT("data\\Roading11.bmp")))
		return false;
	

	HRESULT hResult;
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {			// 플립은 주표면과 보조표면의 주소를 바꾸는것
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	g_TitleSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, g_IpSecondarySurface, false);
	g_LoadingSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2+120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {			
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}
	if (!g_BackgroundSprite.InitSprite(2, 1366, 768, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_BackgroundSprite.LoadFrame(0, TEXT("data\\Background1.bmp")))
		return false;
	if (!g_BackgroundSprite.LoadFrame(1, TEXT("data\\Background2.bmp")))
		return false;

	g_LoadingSprite.Drawing(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {			
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	if (!g_EnemySprite.InitSprite(10, 100, 75, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_EnemySprite.LoadFrame(0, TEXT("data\\enemy_motion0000.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(1, TEXT("data\\enemy_motion0001.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(2, TEXT("data\\enemy_motion0002.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(3, TEXT("data\\enemy_motion0003.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(4, TEXT("data\\enemy_motion0004.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(5, TEXT("data\\enemy_motion0005.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(6, TEXT("data\\enemy_motion0006.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(7, TEXT("data\\enemy_motion0007.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(8, TEXT("data\\enemy_motion0008.bmp")))
		return false;
	if (!g_EnemySprite.LoadFrame(9, TEXT("data\\enemy_motion0009.bmp")))
		return false;

	g_LoadingSprite.Drawing(2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	if (!g_MyShipSprite.InitSprite(8, 56, 76, COLOR_KEY, g_IpDirectDrawObject))
			return false;
	if (!g_MyShipSprite.LoadFrame(0, TEXT("data\\Char_idle.bmp")))
			return false;
	if (!g_MyShipSprite.LoadFrame(1, TEXT("data\\Char_left.bmp")))
			return false;
	if (!g_MyShipSprite.LoadFrame(2, TEXT("data\\Char_right.bmp")))
			return false;
	if (!g_MyShipSprite.LoadFrame(3, TEXT("data\\Char_up.bmp")))
		return false;
	if (!g_MyShipSprite.LoadFrame(4, TEXT("data\\Char_down.bmp")))
		return false;
	if (!g_MyShipSprite.LoadFrame(5, TEXT("data\\Char_atk1.bmp")))
		return false;
	if (!g_MyShipSprite.LoadFrame(6, TEXT("data\\Char_atk2.bmp")))
		return false;
	if (!g_MyShipSprite.LoadFrame(7, TEXT("data\\skillmotion.bmp")))
		return false;
	

	g_LoadingSprite.Drawing(3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	if (!playerMissileSprite.InitSprite(1, 48, 32, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!playerMissileSprite.LoadFrame(0, TEXT("data\\Char_missile.bmp")))
		return false;
	
	g_LoadingSprite.Drawing(4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	if (!g_ExplodingSprite.InitSprite(20, 160, 120, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_ExplodingSprite.LoadFrame(0, TEXT("data\\SimpleExplosion001.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(1, TEXT("data\\SimpleExplosion002.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(2, TEXT("data\\SimpleExplosion003.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(3, TEXT("data\\SimpleExplosion004.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(4, TEXT("data\\SimpleExplosion005.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(5, TEXT("data\\SimpleExplosion006.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(6, TEXT("data\\SimpleExplosion007.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(7, TEXT("data\\SimpleExplosion008.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(8, TEXT("data\\SimpleExplosion009.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(9, TEXT("data\\SimpleExplosion010.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(10, TEXT("data\\SimpleExplosion011.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(11, TEXT("data\\SimpleExplosion012.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(12, TEXT("data\\SimpleExplosion013.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(13, TEXT("data\\SimpleExplosion014.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(14, TEXT("data\\SimpleExplosion015.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(15, TEXT("data\\SimpleExplosion016.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(16, TEXT("data\\SimpleExplosion017.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(17, TEXT("data\\SimpleExplosion018.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(18, TEXT("data\\SimpleExplosion019.bmp")))
		return false;
	if (!g_ExplodingSprite.LoadFrame(19, TEXT("data\\SimpleExplosion020.bmp")))
		return false;

	g_LoadingSprite.Drawing(5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}
	if (!g_SkillSceneSprite.InitSprite(71, 1920, 1080, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(0, TEXT("data\\ani\\01.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(1, TEXT("data\\ani\\02.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(2, TEXT("data\\ani\\03.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(3, TEXT("data\\ani\\04.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(4, TEXT("data\\ani\\05.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(5, TEXT("data\\ani\\06.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(6, TEXT("data\\ani\\07.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(7, TEXT("data\\ani\\08.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(8, TEXT("data\\ani\\09.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(9, TEXT("data\\ani\\10.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(10, TEXT("data\\ani\\11.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(11, TEXT("data\\ani\\12.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(12, TEXT("data\\ani\\13.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(13, TEXT("data\\ani\\14.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(14, TEXT("data\\ani\\15.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(15, TEXT("data\\ani\\16.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(16, TEXT("data\\ani\\17.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(17, TEXT("data\\ani\\18.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(18, TEXT("data\\ani\\19.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(19, TEXT("data\\ani\\20.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(20, TEXT("data\\ani\\21.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(21, TEXT("data\\ani\\22.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(22, TEXT("data\\ani\\23.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(23, TEXT("data\\ani\\24.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(24, TEXT("data\\ani\\25.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(25, TEXT("data\\ani\\26.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(26, TEXT("data\\ani\\27.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(27, TEXT("data\\ani\\28.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(28, TEXT("data\\ani\\29.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(29, TEXT("data\\ani\\30.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(30, TEXT("data\\ani\\31.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(31, TEXT("data\\ani\\32.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(32, TEXT("data\\ani\\33.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(33, TEXT("data\\ani\\34.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(34, TEXT("data\\ani\\35.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(35, TEXT("data\\ani\\36.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(36, TEXT("data\\ani\\37.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(37, TEXT("data\\ani\\38.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(38, TEXT("data\\ani\\39.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(39, TEXT("data\\ani\\40.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(40, TEXT("data\\ani\\41.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(41, TEXT("data\\ani\\42.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(42, TEXT("data\\ani\\43.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(43, TEXT("data\\ani\\44.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(44, TEXT("data\\ani\\45.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(45, TEXT("data\\ani\\46.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(46, TEXT("data\\ani\\47.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(47, TEXT("data\\ani\\48.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(48, TEXT("data\\ani\\49.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(49, TEXT("data\\ani\\50.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(50, TEXT("data\\ani\\51.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(51, TEXT("data\\ani\\52.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(52, TEXT("data\\ani\\53.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(53, TEXT("data\\ani\\54.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(54, TEXT("data\\ani\\55.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(55, TEXT("data\\ani\\56.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(56, TEXT("data\\ani\\57.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(57, TEXT("data\\ani\\58.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(58, TEXT("data\\ani\\59.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(59, TEXT("data\\ani\\60.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(60, TEXT("data\\ani\\61.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(61, TEXT("data\\ani\\62.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(62, TEXT("data\\ani\\63.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(63, TEXT("data\\ani\\64.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(64, TEXT("data\\ani\\65.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(65, TEXT("data\\ani\\66.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(66, TEXT("data\\ani\\67.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(67, TEXT("data\\ani\\68.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(68, TEXT("data\\ani\\69.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(69, TEXT("data\\ani\\70.bmp")))
		return false;
	if (!g_SkillSceneSprite.LoadFrame(70, TEXT("data\\ani\\71.bmp")))
		return false;
	
	g_LoadingSprite.Drawing(6, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	

	g_LoadingSprite.Drawing(7, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

	

	if (!g_EnemyMissileSprite.InitSprite(1, 5, 20, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(0, TEXT("data\\Enemy_Missile.bmp")))
		return false;


	if (!g_Boss1Sprite.InitSprite(1, 61, 91, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss1Sprite.LoadFrame(0, TEXT("data\\Boss1.bmp")))
		return false;

	if (!g_Boss2Sprite.InitSprite(4, 105, 93, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss2Sprite.LoadFrame(0, TEXT("data\\Boss2.bmp")))
		return false;
	if (!g_Boss2Sprite.LoadFrame(1, TEXT("data\\boss2_baseatk.bmp")))
		return false;
	if (!g_Boss2Sprite.LoadFrame(2, TEXT("data\\boss2_atk.bmp")))
		return false;
	if (!g_Boss2Sprite.LoadFrame(3, TEXT("data\\boss2_atk1.bmp")))
		return false;

	if (!g_Boss3Sprite.InitSprite(5, 107, 113, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss3Sprite.LoadFrame(0, TEXT("data\\boss3.bmp")))
		return false;
	if (!g_Boss3Sprite.LoadFrame(1, TEXT("data\\boss3_atk.bmp")))
		return false;
	if (!g_Boss3Sprite.LoadFrame(2, TEXT("data\\boss3_skillatk.bmp")))
		return false;
	if (!g_Boss3Sprite.LoadFrame(3, TEXT("data\\boss3_skillatk1.bmp")))
		return false;
	if (!g_Boss3Sprite.LoadFrame(4, TEXT("data\\boss3_skillmode.bmp")))
		return false;

	if (!g_Boss1MissileSprite.InitSprite(1, 28, 28, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss1MissileSprite.LoadFrame(0, TEXT("data\\boss1_missile.bmp")))
		return false;

	if (!g_Boss3MissileSprite.InitSprite(4, 130, 130, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss3MissileSprite.LoadFrame(0, TEXT("data\\Boss3_bullet.bmp")))
		return false;
	if (!g_Boss3MissileSprite.LoadFrame(1, TEXT("data\\Boss3_bullet2.bmp")))
		return false;
	if (!g_Boss3MissileSprite.LoadFrame(2, TEXT("data\\Boss3_bullet3.bmp")))
		return false;
	if (!g_Boss3MissileSprite.LoadFrame(3, TEXT("data\\Boss3_bullet4.bmp")))
		return false;

	g_LoadingSprite.Drawing(8, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}
	if (!g_EndingSprite.InitSprite(208, 1920, 1080, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_EndingSprite.LoadFrame(0, TEXT("data\\ending\\0.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(1, TEXT("data\\ending\\1.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(2, TEXT("data\\ending\\2.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(3, TEXT("data\\ending\\3.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(4, TEXT("data\\ending\\4.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(5, TEXT("data\\ending\\5.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(6, TEXT("data\\ending\\6.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(7, TEXT("data\\ending\\7.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(8, TEXT("data\\ending\\8.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(9, TEXT("data\\ending\\9.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(10, TEXT("data\\ending\\10.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(11, TEXT("data\\ending\\11.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(12, TEXT("data\\ending\\12.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(13, TEXT("data\\ending\\13.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(14, TEXT("data\\ending\\14.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(15, TEXT("data\\ending\\15.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(16, TEXT("data\\ending\\16.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(17, TEXT("data\\ending\\17.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(18, TEXT("data\\ending\\18.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(19, TEXT("data\\ending\\19.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(20, TEXT("data\\ending\\20.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(21, TEXT("data\\ending\\21.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(22, TEXT("data\\ending\\22.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(23, TEXT("data\\ending\\23.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(24, TEXT("data\\ending\\24.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(25, TEXT("data\\ending\\25.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(26, TEXT("data\\ending\\26.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(27, TEXT("data\\ending\\27.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(28, TEXT("data\\ending\\28.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(29, TEXT("data\\ending\\29.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(30, TEXT("data\\ending\\30.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(31, TEXT("data\\ending\\31.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(32, TEXT("data\\ending\\32.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(33, TEXT("data\\ending\\33.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(34, TEXT("data\\ending\\34.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(35, TEXT("data\\ending\\35.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(36, TEXT("data\\ending\\36.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(37, TEXT("data\\ending\\37.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(38, TEXT("data\\ending\\38.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(39, TEXT("data\\ending\\39.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(40, TEXT("data\\ending\\40.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(41, TEXT("data\\ending\\41.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(42, TEXT("data\\ending\\42.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(43, TEXT("data\\ending\\43.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(44, TEXT("data\\ending\\44.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(45, TEXT("data\\ending\\45.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(46, TEXT("data\\ending\\46.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(47, TEXT("data\\ending\\47.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(48, TEXT("data\\ending\\48.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(49, TEXT("data\\ending\\49.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(50, TEXT("data\\ending\\50.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(51, TEXT("data\\ending\\51.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(52, TEXT("data\\ending\\52.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(53, TEXT("data\\ending\\53.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(54, TEXT("data\\ending\\54.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(55, TEXT("data\\ending\\55.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(56, TEXT("data\\ending\\56.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(57, TEXT("data\\ending\\57.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(58, TEXT("data\\ending\\58.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(59, TEXT("data\\ending\\59.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(60, TEXT("data\\ending\\60.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(61, TEXT("data\\ending\\61.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(62, TEXT("data\\ending\\62.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(63, TEXT("data\\ending\\63.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(64, TEXT("data\\ending\\64.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(65, TEXT("data\\ending\\65.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(66, TEXT("data\\ending\\66.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(67, TEXT("data\\ending\\67.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(68, TEXT("data\\ending\\68.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(69, TEXT("data\\ending\\69.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(70, TEXT("data\\ending\\70.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(71, TEXT("data\\ending\\71.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(72, TEXT("data\\ending\\72.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(73, TEXT("data\\ending\\73.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(74, TEXT("data\\ending\\74.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(75, TEXT("data\\ending\\75.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(76, TEXT("data\\ending\\76.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(77, TEXT("data\\ending\\77.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(78, TEXT("data\\ending\\78.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(79, TEXT("data\\ending\\79.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(80, TEXT("data\\ending\\80.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(81, TEXT("data\\ending\\81.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(82, TEXT("data\\ending\\82.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(83, TEXT("data\\ending\\83.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(84, TEXT("data\\ending\\84.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(85, TEXT("data\\ending\\85.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(86, TEXT("data\\ending\\86.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(87, TEXT("data\\ending\\87.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(88, TEXT("data\\ending\\88.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(89, TEXT("data\\ending\\89.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(90, TEXT("data\\ending\\90.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(91, TEXT("data\\ending\\91.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(92, TEXT("data\\ending\\92.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(93, TEXT("data\\ending\\93.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(94, TEXT("data\\ending\\94.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(95, TEXT("data\\ending\\95.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(96, TEXT("data\\ending\\96.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(97, TEXT("data\\ending\\97.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(98, TEXT("data\\ending\\98.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(99, TEXT("data\\ending\\99.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(100, TEXT("data\\ending\\100.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(101, TEXT("data\\ending\\101.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(102, TEXT("data\\ending\\102.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(103, TEXT("data\\ending\\103.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(104, TEXT("data\\ending\\104.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(105, TEXT("data\\ending\\105.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(106, TEXT("data\\ending\\106.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(107, TEXT("data\\ending\\107.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(108, TEXT("data\\ending\\108.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(109, TEXT("data\\ending\\109.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(110, TEXT("data\\ending\\110.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(111, TEXT("data\\ending\\111.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(112, TEXT("data\\ending\\112.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(113, TEXT("data\\ending\\113.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(114, TEXT("data\\ending\\114.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(115, TEXT("data\\ending\\115.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(116, TEXT("data\\ending\\116.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(117, TEXT("data\\ending\\117.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(118, TEXT("data\\ending\\118.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(119, TEXT("data\\ending\\119.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(120, TEXT("data\\ending\\120.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(121, TEXT("data\\ending\\121.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(122, TEXT("data\\ending\\122.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(123, TEXT("data\\ending\\123.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(124, TEXT("data\\ending\\124.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(125, TEXT("data\\ending\\125.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(126, TEXT("data\\ending\\126.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(127, TEXT("data\\ending\\127.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(128, TEXT("data\\ending\\128.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(129, TEXT("data\\ending\\129.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(130, TEXT("data\\ending\\130.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(131, TEXT("data\\ending\\131.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(132, TEXT("data\\ending\\132.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(133, TEXT("data\\ending\\133.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(134, TEXT("data\\ending\\134.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(135, TEXT("data\\ending\\135.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(136, TEXT("data\\ending\\136.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(137, TEXT("data\\ending\\137.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(138, TEXT("data\\ending\\138.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(139, TEXT("data\\ending\\139.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(140, TEXT("data\\ending\\140.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(141, TEXT("data\\ending\\141.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(142, TEXT("data\\ending\\142.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(143, TEXT("data\\ending\\143.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(144, TEXT("data\\ending\\144.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(145, TEXT("data\\ending\\145.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(146, TEXT("data\\ending\\146.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(147, TEXT("data\\ending\\147.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(148, TEXT("data\\ending\\148.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(149, TEXT("data\\ending\\149.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(150, TEXT("data\\ending\\150.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(151, TEXT("data\\ending\\151.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(152, TEXT("data\\ending\\152.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(153, TEXT("data\\ending\\153.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(154, TEXT("data\\ending\\154.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(155, TEXT("data\\ending\\155.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(156, TEXT("data\\ending\\156.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(157, TEXT("data\\ending\\157.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(158, TEXT("data\\ending\\158.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(159, TEXT("data\\ending\\159.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(160, TEXT("data\\ending\\160.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(161, TEXT("data\\ending\\161.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(162, TEXT("data\\ending\\162.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(163, TEXT("data\\ending\\163.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(164, TEXT("data\\ending\\164.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(165, TEXT("data\\ending\\165.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(166, TEXT("data\\ending\\166.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(167, TEXT("data\\ending\\167.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(168, TEXT("data\\ending\\168.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(169, TEXT("data\\ending\\169.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(170, TEXT("data\\ending\\170.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(171, TEXT("data\\ending\\171.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(172, TEXT("data\\ending\\172.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(173, TEXT("data\\ending\\173.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(174, TEXT("data\\ending\\174.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(175, TEXT("data\\ending\\175.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(176, TEXT("data\\ending\\176.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(177, TEXT("data\\ending\\177.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(178, TEXT("data\\ending\\178.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(179, TEXT("data\\ending\\179.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(180, TEXT("data\\ending\\180.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(181, TEXT("data\\ending\\181.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(182, TEXT("data\\ending\\182.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(183, TEXT("data\\ending\\183.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(184, TEXT("data\\ending\\184.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(185, TEXT("data\\ending\\185.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(186, TEXT("data\\ending\\186.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(187, TEXT("data\\ending\\187.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(188, TEXT("data\\ending\\188.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(189, TEXT("data\\ending\\189.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(190, TEXT("data\\ending\\190.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(191, TEXT("data\\ending\\191.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(192, TEXT("data\\ending\\192.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(193, TEXT("data\\ending\\193.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(194, TEXT("data\\ending\\194.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(195, TEXT("data\\ending\\195.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(196, TEXT("data\\ending\\196.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(197, TEXT("data\\ending\\197.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(198, TEXT("data\\ending\\198.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(199, TEXT("data\\ending\\199.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(200, TEXT("data\\ending\\200.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(201, TEXT("data\\ending\\201.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(202, TEXT("data\\ending\\202.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(203, TEXT("data\\ending\\203.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(204, TEXT("data\\ending\\204.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(205, TEXT("data\\ending\\205.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(206, TEXT("data\\ending\\206.bmp")))
		return false;
	if (!g_EndingSprite.LoadFrame(207, TEXT("data\\ending\\207.bmp")))
		return false;

	if (!g_ClearSprite.InitSprite(1, 400, 150, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_ClearSprite.LoadFrame(0, TEXT("data\\clear.bmp")))
		return false;

	g_LoadingSprite.Drawing(9, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}

    if (!g_DeadSprite.InitSprite(1, 400, 150, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_DeadSprite.LoadFrame(0, TEXT("data\\dead.bmp")))
		return false;

	if (!g_IntroSprite.InitSprite(1, 1366, 768, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_IntroSprite.LoadFrame(0, TEXT("data\\intro.bmp")))
		return false;


	if (!g_GameStartSprite.InitSprite(2, 357, 58, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_GameStartSprite.LoadFrame(0, TEXT("data\\Gamestart.bmp")))
		return false;
	if (!g_GameStartSprite.LoadFrame(1, TEXT("data\\Gamestart1.bmp")))
		return false;

	if (!g_QuitSprite.InitSprite(2, 357, 58, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_QuitSprite.LoadFrame(0, TEXT("data\\Quit.bmp")))
		return false;
	if (!g_QuitSprite.LoadFrame(1, TEXT("data\\Quit1.bmp")))
		return false;

	if (!g_PlayerIconSprite.InitSprite(1, 72, 72, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_PlayerIconSprite.LoadFrame(0, TEXT("data\\icon.bmp")))
		return false;

	if (!g_LifeSprite.InitSprite(1, 72, 72, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_LifeSprite.LoadFrame(0, TEXT("data\\life.bmp")))
		return false;

	if (!g_ItemSprite.InitSprite(3, 32, 32, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_ItemSprite.LoadFrame(0, TEXT("data\\hpPotion.bmp")))
		return false;
	if (!g_ItemSprite.LoadFrame(1, TEXT("data\\mpPotion.bmp")))
		return false;
	if (!g_ItemSprite.LoadFrame(2, TEXT("data\\barrier.bmp")))
		return false;

	if (!g_BarrierEffectSprite.InitSprite(1, 72, 114, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_BarrierEffectSprite.LoadFrame(0, TEXT("data\\buff.bmp")))
		return false;

	if (!g_BossIconSprite.InitSprite(3, 100, 67, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_BossIconSprite.LoadFrame(0, TEXT("data\\boss1_icon.bmp")))
		return false;
	if (!g_BossIconSprite.LoadFrame(1, TEXT("data\\boss2_icon.bmp")))
		return false;
	if (!g_BossIconSprite.LoadFrame(2, TEXT("data\\boss3_icon.bmp")))
		return false;

	if (!g_ItemSlotSprite.InitSprite(1, 65, 65, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_ItemSlotSprite.LoadFrame(0, TEXT("data\\Item slot.bmp")))
		return false;

	if (!g_SkillSprite.InitSprite(7, 1366, 768, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_SkillSprite.LoadFrame(0, TEXT("data\\skill1.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(1, TEXT("data\\skill2.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(2, TEXT("data\\skill3.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(3, TEXT("data\\skill4.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(4, TEXT("data\\skill5.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(5, TEXT("data\\skill6.bmp")))
		return false;
	if (!g_SkillSprite.LoadFrame(6, TEXT("data\\skill7.bmp")))
		return false;

	if (!g_SkillLoadingBarSprite.InitSprite(9, 160, 12, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(0, TEXT("data\\skillLoading1.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(1, TEXT("data\\skillLoading2.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(2, TEXT("data\\skillLoading3.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(3, TEXT("data\\skillLoading4.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(4, TEXT("data\\skillLoading5.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(5, TEXT("data\\skillLoading6.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(6, TEXT("data\\skillLoading7.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(7, TEXT("data\\skillLoading8.bmp")))
		return false;
	if (!g_SkillLoadingBarSprite.LoadFrame(8, TEXT("data\\skillLoading9.bmp")))
		return false;

	if (!g_MaxPowerSprite.InitSprite(1, 100, 30, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_MaxPowerSprite.LoadFrame(0, TEXT("data\\max_power1.bmp")))
		return false;

	if (!g_Boss2MissileSprite.InitSprite(1, 960, 70, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss2MissileSprite.LoadFrame(0, TEXT("data\\boss2_missile.bmp")))
		return false;

	if (!g_Boss3IntroSprite.InitSprite(261, 1920, 1080, COLOR_KEY, g_IpDirectDrawObject))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(0, TEXT("data\\cellAni\\0.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(1, TEXT("data\\cellAni\\1.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(2, TEXT("data\\cellAni\\2.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(3, TEXT("data\\cellAni\\3.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(4, TEXT("data\\cellAni\\4.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(5, TEXT("data\\cellAni\\5.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(6, TEXT("data\\cellAni\\6.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(7, TEXT("data\\cellAni\\7.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(8, TEXT("data\\cellAni\\8.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(9, TEXT("data\\cellAni\\9.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(10, TEXT("data\\cellAni\\10.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(11, TEXT("data\\cellAni\\11.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(12, TEXT("data\\cellAni\\12.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(13, TEXT("data\\cellAni\\13.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(14, TEXT("data\\cellAni\\14.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(15, TEXT("data\\cellAni\\15.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(16, TEXT("data\\cellAni\\16.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(17, TEXT("data\\cellAni\\17.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(18, TEXT("data\\cellAni\\18.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(19, TEXT("data\\cellAni\\19.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(20, TEXT("data\\cellAni\\20.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(21, TEXT("data\\cellAni\\21.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(22, TEXT("data\\cellAni\\22.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(23, TEXT("data\\cellAni\\23.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(24, TEXT("data\\cellAni\\24.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(25, TEXT("data\\cellAni\\25.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(26, TEXT("data\\cellAni\\26.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(27, TEXT("data\\cellAni\\27.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(28, TEXT("data\\cellAni\\28.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(29, TEXT("data\\cellAni\\29.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(30, TEXT("data\\cellAni\\30.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(31, TEXT("data\\cellAni\\31.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(32, TEXT("data\\cellAni\\32.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(33, TEXT("data\\cellAni\\33.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(34, TEXT("data\\cellAni\\34.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(35, TEXT("data\\cellAni\\35.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(36, TEXT("data\\cellAni\\36.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(37, TEXT("data\\cellAni\\37.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(38, TEXT("data\\cellAni\\38.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(39, TEXT("data\\cellAni\\39.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(40, TEXT("data\\cellAni\\40.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(41, TEXT("data\\cellAni\\41.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(42, TEXT("data\\cellAni\\42.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(43, TEXT("data\\cellAni\\43.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(44, TEXT("data\\cellAni\\44.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(45, TEXT("data\\cellAni\\45.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(46, TEXT("data\\cellAni\\46.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(47, TEXT("data\\cellAni\\47.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(48, TEXT("data\\cellAni\\48.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(49, TEXT("data\\cellAni\\49.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(50, TEXT("data\\cellAni\\50.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(51, TEXT("data\\cellAni\\51.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(52, TEXT("data\\cellAni\\52.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(53, TEXT("data\\cellAni\\53.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(54, TEXT("data\\cellAni\\54.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(55, TEXT("data\\cellAni\\55.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(56, TEXT("data\\cellAni\\56.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(57, TEXT("data\\cellAni\\57.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(58, TEXT("data\\cellAni\\58.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(59, TEXT("data\\cellAni\\59.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(60, TEXT("data\\cellAni\\60.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(61, TEXT("data\\cellAni\\61.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(62, TEXT("data\\cellAni\\62.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(63, TEXT("data\\cellAni\\63.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(64, TEXT("data\\cellAni\\64.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(65, TEXT("data\\cellAni\\65.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(66, TEXT("data\\cellAni\\66.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(67, TEXT("data\\cellAni\\67.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(68, TEXT("data\\cellAni\\68.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(69, TEXT("data\\cellAni\\69.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(70, TEXT("data\\cellAni\\70.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(71, TEXT("data\\cellAni\\71.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(72, TEXT("data\\cellAni\\72.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(73, TEXT("data\\cellAni\\73.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(74, TEXT("data\\cellAni\\74.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(75, TEXT("data\\cellAni\\75.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(76, TEXT("data\\cellAni\\76.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(77, TEXT("data\\cellAni\\77.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(78, TEXT("data\\cellAni\\78.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(79, TEXT("data\\cellAni\\79.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(80, TEXT("data\\cellAni\\80.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(81, TEXT("data\\cellAni\\81.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(82, TEXT("data\\cellAni\\82.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(83, TEXT("data\\cellAni\\83.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(84, TEXT("data\\cellAni\\84.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(85, TEXT("data\\cellAni\\85.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(86, TEXT("data\\cellAni\\86.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(87, TEXT("data\\cellAni\\87.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(88, TEXT("data\\cellAni\\88.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(89, TEXT("data\\cellAni\\89.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(90, TEXT("data\\cellAni\\90.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(91, TEXT("data\\cellAni\\91.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(92, TEXT("data\\cellAni\\92.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(93, TEXT("data\\cellAni\\93.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(94, TEXT("data\\cellAni\\94.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(95, TEXT("data\\cellAni\\95.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(96, TEXT("data\\cellAni\\96.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(97, TEXT("data\\cellAni\\97.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(98, TEXT("data\\cellAni\\98.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(99, TEXT("data\\cellAni\\99.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(100, TEXT("data\\cellAni\\100.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(101, TEXT("data\\cellAni\\101.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(102, TEXT("data\\cellAni\\102.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(103, TEXT("data\\cellAni\\103.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(104, TEXT("data\\cellAni\\104.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(105, TEXT("data\\cellAni\\105.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(106, TEXT("data\\cellAni\\106.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(107, TEXT("data\\cellAni\\107.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(108, TEXT("data\\cellAni\\108.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(109, TEXT("data\\cellAni\\109.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(110, TEXT("data\\cellAni\\110.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(111, TEXT("data\\cellAni\\111.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(112, TEXT("data\\cellAni\\112.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(113, TEXT("data\\cellAni\\113.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(114, TEXT("data\\cellAni\\114.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(115, TEXT("data\\cellAni\\115.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(116, TEXT("data\\cellAni\\116.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(117, TEXT("data\\cellAni\\117.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(118, TEXT("data\\cellAni\\118.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(119, TEXT("data\\cellAni\\119.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(120, TEXT("data\\cellAni\\120.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(121, TEXT("data\\cellAni\\121.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(122, TEXT("data\\cellAni\\122.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(123, TEXT("data\\cellAni\\123.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(124, TEXT("data\\cellAni\\124.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(125, TEXT("data\\cellAni\\125.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(126, TEXT("data\\cellAni\\126.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(127, TEXT("data\\cellAni\\127.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(128, TEXT("data\\cellAni\\128.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(129, TEXT("data\\cellAni\\129.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(130, TEXT("data\\cellAni\\130.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(131, TEXT("data\\cellAni\\131.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(132, TEXT("data\\cellAni\\132.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(133, TEXT("data\\cellAni\\133.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(134, TEXT("data\\cellAni\\134.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(135, TEXT("data\\cellAni\\135.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(136, TEXT("data\\cellAni\\136.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(137, TEXT("data\\cellAni\\137.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(138, TEXT("data\\cellAni\\138.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(139, TEXT("data\\cellAni\\139.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(140, TEXT("data\\cellAni\\140.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(141, TEXT("data\\cellAni\\141.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(142, TEXT("data\\cellAni\\142.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(143, TEXT("data\\cellAni\\143.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(144, TEXT("data\\cellAni\\144.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(145, TEXT("data\\cellAni\\145.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(146, TEXT("data\\cellAni\\146.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(147, TEXT("data\\cellAni\\147.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(148, TEXT("data\\cellAni\\148.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(149, TEXT("data\\cellAni\\149.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(150, TEXT("data\\cellAni\\150.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(151, TEXT("data\\cellAni\\151.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(152, TEXT("data\\cellAni\\152.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(153, TEXT("data\\cellAni\\153.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(154, TEXT("data\\cellAni\\154.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(155, TEXT("data\\cellAni\\155.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(156, TEXT("data\\cellAni\\156.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(157, TEXT("data\\cellAni\\157.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(158, TEXT("data\\cellAni\\158.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(159, TEXT("data\\cellAni\\159.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(160, TEXT("data\\cellAni\\160.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(161, TEXT("data\\cellAni\\161.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(162, TEXT("data\\cellAni\\162.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(163, TEXT("data\\cellAni\\163.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(164, TEXT("data\\cellAni\\164.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(165, TEXT("data\\cellAni\\165.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(166, TEXT("data\\cellAni\\166.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(167, TEXT("data\\cellAni\\167.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(168, TEXT("data\\cellAni\\168.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(169, TEXT("data\\cellAni\\169.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(170, TEXT("data\\cellAni\\170.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(171, TEXT("data\\cellAni\\171.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(172, TEXT("data\\cellAni\\172.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(173, TEXT("data\\cellAni\\173.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(174, TEXT("data\\cellAni\\174.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(175, TEXT("data\\cellAni\\175.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(176, TEXT("data\\cellAni\\176.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(177, TEXT("data\\cellAni\\177.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(178, TEXT("data\\cellAni\\178.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(179, TEXT("data\\cellAni\\179.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(180, TEXT("data\\cellAni\\180.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(181, TEXT("data\\cellAni\\181.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(182, TEXT("data\\cellAni\\182.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(183, TEXT("data\\cellAni\\183.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(184, TEXT("data\\cellAni\\184.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(185, TEXT("data\\cellAni\\185.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(186, TEXT("data\\cellAni\\186.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(187, TEXT("data\\cellAni\\187.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(188, TEXT("data\\cellAni\\188.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(189, TEXT("data\\cellAni\\189.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(190, TEXT("data\\cellAni\\190.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(191, TEXT("data\\cellAni\\191.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(192, TEXT("data\\cellAni\\192.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(193, TEXT("data\\cellAni\\193.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(194, TEXT("data\\cellAni\\194.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(195, TEXT("data\\cellAni\\195.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(196, TEXT("data\\cellAni\\196.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(197, TEXT("data\\cellAni\\197.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(198, TEXT("data\\cellAni\\198.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(199, TEXT("data\\cellAni\\199.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(200, TEXT("data\\cellAni\\200.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(201, TEXT("data\\cellAni\\201.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(202, TEXT("data\\cellAni\\202.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(203, TEXT("data\\cellAni\\203.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(204, TEXT("data\\cellAni\\204.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(205, TEXT("data\\cellAni\\205.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(206, TEXT("data\\cellAni\\206.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(207, TEXT("data\\cellAni\\207.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(208, TEXT("data\\cellAni\\208.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(209, TEXT("data\\cellAni\\209.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(210, TEXT("data\\cellAni\\210.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(211, TEXT("data\\cellAni\\211.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(212, TEXT("data\\cellAni\\212.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(213, TEXT("data\\cellAni\\213.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(214, TEXT("data\\cellAni\\214.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(215, TEXT("data\\cellAni\\215.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(216, TEXT("data\\cellAni\\216.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(217, TEXT("data\\cellAni\\217.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(218, TEXT("data\\cellAni\\218.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(219, TEXT("data\\cellAni\\219.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(220, TEXT("data\\cellAni\\220.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(221, TEXT("data\\cellAni\\221.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(222, TEXT("data\\cellAni\\222.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(223, TEXT("data\\cellAni\\223.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(224, TEXT("data\\cellAni\\224.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(225, TEXT("data\\cellAni\\225.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(226, TEXT("data\\cellAni\\226.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(227, TEXT("data\\cellAni\\227.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(228, TEXT("data\\cellAni\\228.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(229, TEXT("data\\cellAni\\229.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(230, TEXT("data\\cellAni\\230.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(231, TEXT("data\\cellAni\\231.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(232, TEXT("data\\cellAni\\232.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(233, TEXT("data\\cellAni\\233.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(234, TEXT("data\\cellAni\\234.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(235, TEXT("data\\cellAni\\235.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(236, TEXT("data\\cellAni\\236.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(237, TEXT("data\\cellAni\\237.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(238, TEXT("data\\cellAni\\238.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(239, TEXT("data\\cellAni\\239.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(240, TEXT("data\\cellAni\\240.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(241, TEXT("data\\cellAni\\241.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(242, TEXT("data\\cellAni\\242.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(243, TEXT("data\\cellAni\\243.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(244, TEXT("data\\cellAni\\244.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(245, TEXT("data\\cellAni\\245.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(246, TEXT("data\\cellAni\\246.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(247, TEXT("data\\cellAni\\247.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(248, TEXT("data\\cellAni\\248.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(249, TEXT("data\\cellAni\\249.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(250, TEXT("data\\cellAni\\250.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(251, TEXT("data\\cellAni\\251.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(252, TEXT("data\\cellAni\\252.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(253, TEXT("data\\cellAni\\253.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(254, TEXT("data\\cellAni\\254.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(255, TEXT("data\\cellAni\\255.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(256, TEXT("data\\cellAni\\256.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(257, TEXT("data\\cellAni\\257.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(258, TEXT("data\\cellAni\\258.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(259, TEXT("data\\cellAni\\259.bmp")))
		return false;
	if (!g_Boss3IntroSprite.LoadFrame(260, TEXT("data\\cellAni\\260.bmp")))
		return false;

	g_LoadingSprite.Drawing(10, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, g_IpSecondarySurface, true);
	if (FAILED(hResult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) {
		if (hResult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}
	return true;
}
