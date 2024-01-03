#include <windows.h>
#include <windowsx.h>

#include <dsound.h>
#include "Dsutil.h"

#include <ddraw.h>
#include <dinput.h>

#include "define.h"

bool InitDirectDraw(void);
bool InitDirectInput(void);

extern HWND g_hwnd;
extern HINSTANCE g_hInstance;

extern LPDIRECTDRAW7 g_IpDirectDrawObject;
extern LPDIRECTDRAWSURFACE7 g_IpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;

extern LPDIRECTINPUT8 g_IpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard;

LPDIRECTSOUND SoundOBJ;
LPDIRECTSOUNDBUFFER SoundDSB;
DSBUFFERDESC DSB_Desc;
HSNDOBJ Sound[17];

//****Surface : 표면
//            : Primary Surface(주표면), Secondary Surface(보조표면), OffScreen(오프스크린 - 3차표면)으로 나눌 수 있다.
//            : Primary Surface(주표면, 전위면) : 단 하나만 생성할 수 있으며, VRAM(비디오메모리)에 생성된다.
//                                             : 주표면의 크기는 설정된 해상도와 일치해야 한다. (예를 들면, 비디오 모드 설정에서 해상도를 800 * 600으로 설정했다면 주표면의 크기도 800 * 600이여야 한다.)
//            : Secondary Surface(보조표면, 후위면) : 후면버퍼, 2차표면이라 칭하기도 한다. 주표면과 같은 구조를 가지며, 주표면이 있어야 생성이 가능하고, 주표면과 플리핑을 한다.
//            : OffScreen(3차표면) : VRAM, RAM(시스템 메모리)에 모두 생성이 가능하고, 크기에 제한은 없다. 주로 비트맵 등의 이미지를 저장하는데 쓰인다.


bool InitializeDirectX(void) {
	if (!InitDirectDraw())
		return false;

	if (!InitDirectInput())
		return false;
	return true;
}

bool InitDirectDraw(void)
{
	if (FAILED(DirectDrawCreateEx(NULL, (LPVOID*)&g_IpDirectDrawObject, IID_IDirectDraw7, NULL)))
		return false;

	if (FAILED(g_IpDirectDrawObject->SetCooperativeLevel(g_hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))
		return false;

	if (FAILED(g_IpDirectDrawObject->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH, 0, 0)))
		return false;

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	if (FAILED(g_IpDirectDrawObject->CreateSurface(&ddsd, &g_IpPrimarySurface, NULL)))
		return false;

	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(g_IpPrimarySurface->GetAttachedSurface(&ddscaps, &g_IpSecondarySurface)))
		return false;

	LPDIRECTDRAWCLIPPER IpClipper;

	if (FAILED(g_IpDirectDrawObject->CreateClipper(NULL, &IpClipper, NULL)))
		return false;

	if (FAILED(IpClipper->SetHWnd(NULL, g_hwnd)))
		return false;

	if (FAILED(g_IpSecondarySurface->SetClipper(IpClipper)))
		return false;
	return true;
}

bool InitDirectInput(void) {
	if (FAILED(DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_IpDirectInputObject, NULL)))
		return false;
	if (FAILED(g_IpDirectInputObject->CreateDevice(GUID_SysKeyboard, &g_IpDirectInputKeyboard, NULL)))
		return false;
	if (FAILED(g_IpDirectInputKeyboard->SetCooperativeLevel(g_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;
	if (FAILED(g_IpDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	g_IpDirectInputKeyboard->Acquire();
	return true;
}

BOOL _InitDirectSound(void)
{
	if (DirectSoundCreate(NULL, &SoundOBJ, NULL) == DS_OK)
	{
		if (SoundOBJ->SetCooperativeLevel(g_hwnd, DSSCL_PRIORITY) != DS_OK)
			return FALSE;

		memset(&DSB_Desc, 0, sizeof(DSBUFFERDESC));
		DSB_Desc.dwSize = sizeof(DSBUFFERDESC);
		DSB_Desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

		if (SoundOBJ->CreateSoundBuffer(&DSB_Desc, &SoundDSB, NULL) != DS_OK)
			return FALSE;

		SoundDSB->SetVolume(0);
		SoundDSB->SetPan(0);

		Sound[0] = SndObjCreate(SoundOBJ, TEXT("sound\\bg.wav"), 1);
		Sound[1] = SndObjCreate(SoundOBJ, TEXT("sound\\atk.wav"), 2);
		Sound[2] = SndObjCreate(SoundOBJ, TEXT("sound\\intro.wav"), 3);
		Sound[3] = SndObjCreate(SoundOBJ, TEXT("sound\\select.wav"), 4);
		Sound[4] = SndObjCreate(SoundOBJ, TEXT("sound\\barrier.wav"), 5);
		Sound[5] = SndObjCreate(SoundOBJ, TEXT("sound\\mpFull.wav"), 6);
		Sound[6] = SndObjCreate(SoundOBJ, TEXT("sound\\boss1Kill.wav"), 7);
		Sound[7] = SndObjCreate(SoundOBJ, TEXT("sound\\boss2Kill.wav"), 8);
		Sound[8] = SndObjCreate(SoundOBJ, TEXT("sound\\skill1.wav"), 9);
		Sound[9] = SndObjCreate(SoundOBJ, TEXT("sound\\skill2.wav"), 10);
		Sound[10] = SndObjCreate(SoundOBJ, TEXT("sound\\final flash.wav"), 11);
		Sound[11] = SndObjCreate(SoundOBJ, TEXT("sound\\dead.wav"), 12);
		Sound[12] = SndObjCreate(SoundOBJ, TEXT("sound\\retry.wav"), 13);
		Sound[13] = SndObjCreate(SoundOBJ, TEXT("sound\\restore.wav"), 14);
		Sound[14] = SndObjCreate(SoundOBJ, TEXT("sound\\cellIntro.wav"), 15);
		Sound[15] = SndObjCreate(SoundOBJ, TEXT("sound\\boss3Bg.wav"), 1);
		Sound[16] = SndObjCreate(SoundOBJ, TEXT("sound\\Ending.wav"), 16);


		return TRUE;
	}
	return FALSE;
}
