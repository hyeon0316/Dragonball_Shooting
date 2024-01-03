#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include "Dsutil.h"
#include "define.h"
#include "Sprite.h"
#include "GameEnum.h"
#include "Timer.h"

void InitIntro();

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_IpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern LPDIRECTINPUT8 g_IpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard;

extern EMod curMod;
extern bool isGameFirst;
extern HSNDOBJ Sound[17];
extern Sprite g_IntroSprite;
extern Sprite g_GameStartSprite;
extern Sprite g_QuitSprite;

bool m_bIntroFirst = true;
system_clock::time_point m_SelectDelayTimer;
int state;

void IntroMain()
{
	if (m_bIntroFirst) InitIntro();

	g_IntroSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, g_IpSecondarySurface, true);//타이틀 화면
	if (Timer::Elapsed(m_SelectDelayTimer, 100))//W or S 입력, elapsed 함수로 경과된 시간을 구해 그 시간 동안은 키 입력을 받지 않음.(빠르게 버튼 이동 방지)
	{
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_W) || (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_S)))
		{
			state += 1;
			SndObjPlay(Sound[3], NULL);
		}
	}

	//state값을 2로 나눈 나머지 값을 두개의 경우로 나눠서 시작 or 종료 버튼 선택
	if (state % 2 == 0)
	{
		g_GameStartSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 180, g_IpSecondarySurface, true);
		g_QuitSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240, g_IpSecondarySurface, true);
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_RETURN))
		{
			curMod = EMod::Game;
			SndObjStop(Sound[2]);
		}
	}
	else
	{
		g_GameStartSprite.Drawing(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 180, g_IpSecondarySurface, true);
		g_QuitSprite.Drawing(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240, g_IpSecondarySurface, true);
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_RETURN))
		{
			curMod = EMod::Quit;
			PostQuitMessage(0);
		}
	}

	HRESULT hRresult;
	if (FAILED(hRresult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) //화면 전환
	{
		if (hRresult == DDERR_SURFACELOST)
			g_IpPrimarySurface->Restore();
	}
}

void InitIntro()
{
	SndObjPlay(Sound[2], DSBPLAY_LOOPING);
	m_SelectDelayTimer =Timer::Now();
	state = 0;
	m_bIntroFirst = false;
	isGameFirst = true;
}