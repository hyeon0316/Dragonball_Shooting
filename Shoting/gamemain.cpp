#pragma once
#include<ddraw.h>
#include<dinput.h>

#include"CBMP.h"
#include"CTimer.h"
#include"CSprite.h"
#include"CEnemy.h"
#include"myship.h"
#include"mymissile.h"
#include"CExploding.h"
#include"CBoss.h"
#include"CBossMissile.h"
#include"CEnemyMissile.h"
#include"item.h"
#include "CGObject.h"
#include "CAnimation.h"

#include<stdio.h>
#include<Windows.h>
#include <time.h>

#include<dsound.h>
#include"Dsutil.h"

#include"define.h"



#define MY_MISSILE_INTRVAL 240
#define BOSS_MISSILE_INTERVAL 100

BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 IpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_IpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern LPDIRECTINPUT g_IpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard;
extern CTimer g_Timer;

extern CSprite g_Boss3MissileSprite;
extern CAnimation g_Ending[MAX_SECENE];
extern CSprite g_EndingSprite;
extern CAnimation g_Boss3Intro[MAX_SECENE];
extern CSprite g_Boss3IntroSprite;
extern CSprite g_BackgroundSprite;
extern CBossMissile g_Boss1Missile[MAX_BOSS_YMISSILES][MAX_BOSS_XMISSILES];
extern CSprite g_Boss1MissileSprite;
extern CBoss g_Boss1[MAX_BOSS];
extern CBoss g_Boss2[MAX_BOSS];
extern CBoss g_Boss3[MAX_BOSS];
extern CSprite g_Boss1Sprite;

extern CEnemyMissile g_EnemyMissile[MAX_XENEMYS][MAX_YENEMYS];
extern CBossMissile g_Boss3Missile[MAX_BOSS3_YMISSILES][MAX_BOSS3_XMISSILES];
extern CSprite g_EnemyMissileSprite;
extern CEnemy g_Enemy[MAX_XENEMYS][MAX_YENEMYS];
extern CSprite g_EnemySprite;

extern CSprite g_Boss2MissileSprite;
extern CAnimation g_Boss2Missile[MAX_SECENE];

extern CSprite g_SkillSceneSprite;
extern CAnimation g_SkillScene[MAX_SECENE];

extern CSprite g_SkillSprite;
extern CAnimation g_Skill[MAX_SECENE];

extern int g_entireTime;
extern int g_entireTime2;

extern int g_Boss2Move;

extern CSprite g_SkillLoadingBarSprite;

extern CSprite g_MaxPowerSprite;

extern CSprite g_Boss3Sprite;
extern CSprite g_Boss2Sprite;
extern CMyShip g_MyShip;
extern CSprite g_MyShipSprite;
extern CMyMissile g_MyMissile[MAX_MISSILES];
extern CSprite g_MyMissileSprite;
extern CExploding g_Exploding[MAX_EXPLODES];
extern CSprite g_ExplodingSprite;

extern CSprite g_ClearSprite;
extern CSprite g_DeadSprite;
extern CSprite g_IntroSprite;

extern CSprite g_GameStartSprite;
extern CSprite g_QuitSprite;

extern CSprite g_PlayerIconSprite;
extern CSprite g_BossIconSprite;
extern CSprite g_LifeSprite;
extern CSprite g_ItemSlotSprite;

extern Item g_Item[MAX_ITEM];
extern CSprite g_ItemSprite;

extern CGObject g_CGObject;

extern CSprite g_BarrierEffectSprite;

extern HSNDOBJ Sound[17];
extern void _Play(int num);

static BOOL m_bGameFirst = TRUE;
static BOOL	m_bGameDead = FALSE;
static BOOL m_bGameClear = FALSE;
static BOOL m_bBoss = FALSE;
static BOOL m_bBoss2 = FALSE;
static BOOL m_bBoss3 = FALSE;
int enemyCnt = MAX_XENEMYS * MAX_YENEMYS;

void Intro();

int g_Introtimestart;

void InitGame();
void MyShipDead();
void MyShipWin();

int g_BarrierTimeStart;
int g_Boss2Page;
int g_Boss3Page;

int g_SkillTimeStart;

bool flagattack = false;
bool attacking = false;
bool skilling = false;
int nMyShipFrame = 0;
int nBossFrame = 0;
int state = 2;
int skillBarState = 0;


bool intro = true;
bool itemOn = false;

bool item_first = false;
bool item_second = false;
bool item_third = false;

bool barrierDraw = false;

bool skillSceneOn = false;
bool skillAttackOn = false;

bool MaxPowerOn = false;

bool Boss2On = false;
bool Boss3On = false;

bool Boss2PageTimer = false;
bool Boss3PageTimer = false;

extern bool boss2Missile;
extern bool boss3Missile;

bool introSoundOn = false;

bool mpSoundOn = false;

bool skillSoundOn1 = false;

bool skillSoundOn2 = false;

bool barrierSoundOn = false;

bool skillAtkBoss = false;
bool skillAtkBoss2 = false;

extern bool startMove;

extern bool skillAtkEn;

extern bool boss2MissileAtk;
extern bool boss3MissileAtk;

bool Boss2Kill = false;

int g_Boss3IntroTimer;

int g_EndingTimer;

bool cellIntroSoundOn = false;

bool boss3BgM = false;

bool boss3BackGround = false;

bool EndingOn = false;

bool EndingSound = false;
void GameMain(void) {
	if (intro)
	{
		Intro();
	}
	else
	{
		if (m_bGameFirst == TRUE)InitGame();
		int i, j, m, m2, k;
		static int time = 0; //처음에만 초기화됨
		static int time2 = 0;
		static int nLastMissileTime = g_Timer.time();
		static int nLastMissileTime2 = g_Timer.time();

		if (!boss3BackGround)
		{
			g_BackgroundSprite.Drawing2(0, 400, 300, g_IpSecondarySurface, false);
		}
		if (boss3BackGround)
		{
			g_BackgroundSprite.Drawing2(1, 400, 300, g_IpSecondarySurface, false);
		}
		g_PlayerIconSprite.Drawing(0,45,40, g_IpSecondarySurface, false);

		if (barrierDraw == true)
		{
			g_BarrierEffectSprite.Drawing(0, g_MyShip.GetX()-10,g_MyShip.GetY()-10, g_IpSecondarySurface, true);
		}
		for (i = 0; i < 3; i++)
		{
			g_ItemSlotSprite.Drawing(0, 45+70*i, 720, g_IpSecondarySurface, true);
		}

		if (enemyCnt == 0)//일반 적이 모두 파괴될 시 첫번째 보스 등장
			m_bBoss = TRUE;

		if (m_bBoss == TRUE)
		{
			for (i = 0; i < MAX_BOSS; i++)
			{
				if (g_Boss1[i].IsLive())
				{
					if (startMove)
					{
						if ((g_MyShip.GetX() - g_Boss1[i].GetX()) > 1000)
							g_Boss1[i].m_speedy = 2;
						if ((g_MyShip.GetX() - g_Boss1[i].GetX()) < -1000)
							g_Boss1[i].m_speedy = -2;
						g_Boss1[i].m_y += g_Boss1[i].m_speedy;
					}
					g_Boss1[i].Move();//보스의 이동 함수
					g_Boss1[i].Draw(g_IpSecondarySurface);//보스를 그려주는 함수
					g_BossIconSprite.Drawing(0, 1310, 35, g_IpSecondarySurface, true);//오른쪽 상단에 보스를 나타내는 아이콘 표시
				}
				if (startMove)
				{
					for (m = 0; m < MAX_BOSS_YMISSILES; m++)
					{
						if (time % 51 == 0)
						{
							for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
							{
								if (!g_Boss1Missile[m][m2].IsLive() && g_Boss1[i].IsLive())
								{
									g_Boss1Missile[m][m2].Initialize(&g_Boss1MissileSprite, g_Boss1[i].GetX() - 80, g_Boss1[i].GetY(), &g_Timer, 0, 10, 5);
									g_Boss1Missile[m][0].m_speedy = -2;
									g_Boss1Missile[m][1].m_speedy = 0;
									g_Boss1Missile[m][2].m_speedy = 2;
									//3방향으로 미사일 발사
								}
							}
						}
						time++;
					}
				}
			}
		}
		if (m_bBoss2 == TRUE)//첫번째 보스가 사망할때 활성화
		{
			for (i = 0; i < MAX_BOSS; i++)
			{	
					if (g_Boss2[i].IsLive() && g_Boss2[i].m_Enegy > 15)//첫 번째 패턴 활성화
					{
						if (startMove)
						{
							if ((g_MyShip.GetX() - g_Boss2[i].GetX()) > 1000)
								g_Boss2[i].m_speedy = 2;
							if ((g_MyShip.GetX() - g_Boss2[i].GetX()) < -1000)
								g_Boss2[i].m_speedy = -2;
							g_Boss2[i].m_y += g_Boss2[i].m_speedy;
						}
						g_Boss2[i].Move();
						g_Boss2[i].DrawFrame(g_IpSecondarySurface, nBossFrame);
						g_BossIconSprite.Drawing(1, 1310, 35, g_IpSecondarySurface, true);
						if (startMove)
						{
							for (m = 0; m < MAX_BOSS_YMISSILES; m++)
							{
								if (time2 % 51 == 0)
								{
									for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
									{
										if (!g_Boss1Missile[m][m2].IsLive() && g_Boss2[i].IsLive())
										{
											g_Boss1Missile[m][m2].Initialize(&g_Boss1MissileSprite, g_Boss2[i].GetX() - 80, g_Boss2[i].GetY(), &g_Timer, 0, 10, 5);
											g_Boss1Missile[m][0].m_speedy = -2;
											g_Boss1Missile[m][1].m_speedy = 0;
											g_Boss1Missile[m][2].m_speedy = 2;
											nBossFrame = 1;
										}
									}
								}
								time2++;
							}
						}
				}
				if (g_Boss2[i].IsLive() && g_Boss2[i].m_Enegy <= 15)//두 번째 패턴 활성화(CBoss.cpp에 설정된 패턴 실행)
				{
					skillAtkEn = true;
					g_BossIconSprite.Drawing(1, 1310, 35, g_IpSecondarySurface, true);
					if (!Boss2PageTimer)
					{
						g_Boss2Page = g_Timer.time();
						g_entireTime = g_Timer.time() - 1500;
						Boss2PageTimer = true;
					}
					if (g_Timer.elapsed(g_Boss2Page, 1500))
					{
						Boss2On = true;
					}
					if (Boss2On)
					{
						g_Boss2[i].DrawFrame(g_IpSecondarySurface, nBossFrame);//에너지파 발사 모션
						g_Boss2[i].Move2();
					}
					if (boss2Missile)
					{
						for (i = 0; i < MAX_SECENE; i++)
						{
							g_Boss2Missile[i].Initialize(&g_Boss2MissileSprite, g_Boss2[i].GetX() - 515, g_Boss2[i].GetY() - 15, &g_Timer, 0, 50);
							g_Boss2Missile[i].Drawskill(g_IpSecondarySurface);//에너지파 발사
						}
					}
				}
			}
		}
		if (m_bBoss3 == TRUE)
		{
			for (i = 0; i < MAX_BOSS; i++)
			{
				if (g_Boss3[i].IsLive() && g_Boss3[i].m_Enegy > 15)
				{
					if (startMove)
					{
						if ((g_MyShip.GetX() - g_Boss3[i].GetX()) > 1000)
							g_Boss3[i].m_speedy = 2;
						if ((g_MyShip.GetX() - g_Boss3[i].GetX()) < -1000)
							g_Boss3[i].m_speedy = -2;
						g_Boss3[i].m_y += g_Boss3[i].m_speedy;
					}
					g_Boss3[i].Move();
					g_Boss3[i].DrawFrame(g_IpSecondarySurface, nBossFrame);
					g_BossIconSprite.Drawing(2, 1310, 32, g_IpSecondarySurface, true);
					if (startMove)
					{
						for (m = 0; m < MAX_BOSS_YMISSILES; m++)
						{
							if (time2 % 51 == 0)
							{
								for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
								{
									if (!g_Boss1Missile[m][m2].IsLive() && g_Boss3[i].IsLive())
									{
										g_Boss1Missile[m][m2].Initialize(&g_Boss1MissileSprite, g_Boss3[i].GetX() - 80, g_Boss3[i].GetY(), &g_Timer, 0, 10, 5);
										g_Boss1Missile[m][0].m_speedy = -2;
										g_Boss1Missile[m][1].m_speedy = 0;
										g_Boss1Missile[m][2].m_speedy = 2;
										nBossFrame = 1;
									}
								}
							}
							time2++;
						}
					}
				}
				else if (g_Boss3[i].IsLive() && ((g_Boss3[i].m_Enegy <= 20) && (g_Boss3[i].m_Enegy > 10)))
				{
					skillAtkEn = true;
					g_BossIconSprite.Drawing(2, 1310, 32, g_IpSecondarySurface, true);
					if (Boss2PageTimer == false)
					{
						g_Boss2Page = g_Timer.time();
						g_entireTime = g_Timer.time() - 1500;
						Boss2PageTimer = true;
					}
					if (g_Timer.elapsed(g_Boss2Page, 1500))
					{
						Boss2On = true;
					}
					if (Boss2On)
					{
						g_Boss3[i].DrawFrame(g_IpSecondarySurface, nBossFrame);
						g_Boss3[i].Move2();
					}
					if (boss2Missile)
					{
						for (i = 0; i < MAX_SECENE; i++)
						{
							g_Boss2Missile[i].Initialize(&g_Boss2MissileSprite, g_Boss3[i].GetX() - 515, g_Boss3[i].GetY() - 15, &g_Timer, 0, 50);
							g_Boss2Missile[i].Drawskill(g_IpSecondarySurface);
						}
					}
				}
				else if (g_Boss3[i].IsLive() && g_Boss3[i].m_Enegy <= 10)
				{
					skillAtkEn = true;
					g_BossIconSprite.Drawing(2, 1310, 32, g_IpSecondarySurface, true);
					if (Boss3PageTimer == false)
					{
						g_Boss3Page = g_Timer.time();
						g_entireTime2 = g_Timer.time() - 1500;
						Boss3PageTimer = true;
					}
					if (g_Timer.elapsed(g_Boss3Page, 1500))
					{
						Boss3On = true;
					}
					if (Boss3On)
					{
						g_Boss3[i].Move3();
						g_Boss3[i].DrawFrame(g_IpSecondarySurface, nBossFrame);
					}
					if (boss3Missile)
					{
						for (m = 0; m < MAX_BOSS3_YMISSILES; m++)
						{
							int boss3RandMill = rand() % 1367;
							if (time2 % 11 == 0)
							{
								for (m2 = 0; m2 < MAX_BOSS3_XMISSILES; m2++)
								{
									if (!g_Boss3Missile[m][m2].IsLive() && g_Boss3[i].IsLive())
									{
										g_Boss3Missile[m][m2].Initialize(&g_Boss3MissileSprite, boss3RandMill, 0, &g_Timer, 0, 50, 5);	
										boss3MissileAtk = true;
									}
								}
							}
							time2++;
						}
					}
				}
			}
		}

		for (i = 0; i < MAX_XENEMYS; i++)
		{
			for (j = 0; j < MAX_YENEMYS; j++)
			{
				if (g_Enemy[i][j].IsLive())
				{
					g_Enemy[i][j].Move();
					g_Enemy[i][j].Draw(g_IpSecondarySurface);

				}
				int nansu = rand() % 150;
				if (nansu == 0)
				{
					if (!g_EnemyMissile[i][j].IsLive() && g_Enemy[i][j].IsLive())
					{
						g_EnemyMissile[i][j].Initialize(&g_EnemyMissileSprite, g_Enemy[i][j].GetX() - 20, g_Enemy[i][j].GetY(), &g_Timer, 0, 150, 5);
					}
				}
			}
		}

		for (m = 0; m < MAX_BOSS_YMISSILES; m++)
		{
			for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
				if (g_Boss1Missile[m][m2].IsLive())
				{
					g_Boss1Missile[m][m2].Move();
					g_Boss1Missile[m][m2].Draw(g_IpSecondarySurface);
				}
			}
		}

		for (m = 0; m < MAX_BOSS3_YMISSILES; m++)
		{
			for (m2 = 0; m2 < MAX_BOSS3_XMISSILES; m2++) {
				if (g_Boss3Missile[m][m2].IsLive())
				{
					g_Boss3Missile[m][m2].Move3();
					g_Boss3Missile[m][m2].Draw(g_IpSecondarySurface);
				}
			}
		}

		for (m = 0; m < MAX_MISSILES; m++)
		{
			if (g_MyMissile[m].IsLive())
			{
				g_MyMissile[m].Move();
				g_MyMissile[m].Draw(g_IpSecondarySurface);
			}
		}
		for (i = 0; i < MAX_XENEMYS; i++)
		{
			for (j = 0; j < MAX_YENEMYS; j++)
			{
				if (g_EnemyMissile[i][j].IsLive())
				{
					g_EnemyMissile[i][j].Move();
					g_EnemyMissile[i][j].Draw(g_IpSecondarySurface);
				}
			}
		}

		if (!attacking)
			nMyShipFrame = 0; //main.cpp에서 프레임 번호
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_A))
		{
			g_MyShip.Left();
			nMyShipFrame = 1;
		}
		else if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_D))
		{
			g_MyShip.Right();
			nMyShipFrame = 2;
		}
		else if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_W))
		{
			g_MyShip.Up();
			nMyShipFrame = 3;
		}
		else if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_S))
		{
			g_MyShip.Down();
			nMyShipFrame = 4;
		}
		attacking = false;
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_SPACE))
		{
			if (g_MyShip.IsLive())
			{
				attacking = true;
				if (g_Timer.elapsed(nLastMissileTime, MY_MISSILE_INTRVAL))
				{
					for (m = 0; m < MAX_MISSILES; m++)
					{
						if (!g_MyMissile[m].IsLive())
							break;
					}
					if (m == MAX_MISSILES)
						return;
					nMyShipFrame = flagattack ? 5 : 6;//두개의 동작 반복
					flagattack = !flagattack;
					g_MyMissile[m].Initialize(&g_MyMissileSprite, g_MyShip.GetX(), g_MyShip.GetY() - 20,
						&g_Timer, 0, 10, 5);//플레이어의 미사일의 발사 시작좌표, 속도(프레임 간격10, 시간 간격5)
					_Play(1);
				}
			}
		}	  
		g_MyShip.DrawFrame(g_IpSecondarySurface, nMyShipFrame);


		for (m = 0; m < MAX_MISSILES; m++)
		{
			for (i = 0; i < MAX_XENEMYS; i++)
			{
				for (j = 0; j < MAX_YENEMYS; j++)
				{
					if (g_MyMissile[m].IsLive() && g_Enemy[i][j].IsLive())
					{
						if (abs(g_MyMissile[m].GetY() - g_Enemy[i][j].GetY()) < 30)
							if (abs(g_MyMissile[m].GetX() - g_Enemy[i][j].GetX()) < 30)
							{
								g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(),
									g_MyMissile[m].GetY(), &g_Timer, 0, 50);
								g_MyMissile[m].Kill();
								g_Enemy[i][j].Kill();
								enemyCnt--;
								++g_MyShip.mp;
								for (k = 0; k < MAX_ITEM; k++)
								{		
									if (!g_Item[k].IsLive())
									{			
										g_Item[k].n_currentItem = rand() % 3;
										g_Item[k].Initialize(&g_ItemSprite, g_Enemy[i][j].GetX(), g_Enemy[i][j].GetY(), &g_Timer, 50,5);
										break;
									}
								}
							}
					}
				}	
			}	
		}
		
		for (k = 0; k < MAX_ITEM; k++)
		{
			g_Item[k].Draw(g_IpSecondarySurface);
			g_Item[k].Move();
			if (g_Item[k].IsLive() && g_MyShip.IsLive())
			{			
				if (abs(g_Item[k].GetY() - g_MyShip.GetY()) < 30)
					if (abs(g_Item[k].GetX() - g_MyShip.GetX()) < 30)
					{
						if (!item_first)
						{
							if (g_Item[k].n_currentItem == 0)
							{
								item_first = true;
								g_Item[k].Kill();
							}
						}
						if (!item_second)
						{
							if (g_Item[k].n_currentItem == 1)
							{
								item_second = true;
								g_Item[k].Kill();
							}
						}
						if (!item_third)
						{
							if (g_Item[k].n_currentItem == 2)
							{
								item_third = true;
								g_Item[k].Kill();
							}
						}
					}
			}
		}

	    if (item_first == true)
		{
			g_ItemSprite.Drawing(0, 45, 720, g_IpSecondarySurface, true);
			if (g_MyShip.hp < 5)
			{
				if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_1))
				{
					g_Item[k].HpUp(3);
					_Play(13);
					item_first = false;
				}
			}
		}
		if (item_second == true)
		{
			g_ItemSprite.Drawing(1, 115, 720, g_IpSecondarySurface, true);
			if (g_MyShip.mp < 8)
			{
				if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_2))
				{
					g_Item[k].MpUp(9);
					item_second = false;
				}

			}
		}
		if (item_third == true)
		{
				g_ItemSprite.Drawing(2, 185, 720, g_IpSecondarySurface, true);
				if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_3))
				{
					g_BarrierTimeStart = g_Timer.time();
					barrierDraw = true;	
					if (barrierSoundOn == false)
					{
						_Play(4);
						barrierSoundOn = true;
					}
				}	
		}

		if (barrierDraw == true)
		{
			if (g_Timer.elapsed(g_BarrierTimeStart, 5000))
			{
				barrierDraw = false;
				item_third = false;
				barrierSoundOn = false;
			}
		}
	
		for (m = 0; m < MAX_EXPLODES; m++)
		{
			for (i = 0; i < MAX_XENEMYS; i++)
			{
				for (j = 0; j < MAX_YENEMYS; j++)
				{
					if (g_EnemyMissile[i][j].IsLive() && g_MyShip.IsLive())
					{
						if (abs(g_EnemyMissile[i][j].GetX() - g_MyShip.GetX()) < 30)
							if (abs(g_EnemyMissile[i][j].GetY() - g_MyShip.GetY()) < 20)
							{
								g_Exploding[m].Initialize(&g_ExplodingSprite, g_EnemyMissile[i][j].GetX(),
									g_EnemyMissile[i][j].GetY(), &g_Timer, 0, 50);
								g_EnemyMissile[i][j].Kill();
								
								if (!skillSceneOn)
								{
									if (!barrierDraw)				
										g_MyShip.hp -= 1;
								}
							
								if (g_MyShip.hp == 0)
								{
									g_MyShip.Kill();
									_Play(11);
									MyShipDead();
								}
							}
					}
				}
			}
			for (k = 0; k < MAX_SECENE; k++)
			{
				if (g_MyShip.IsLive()&& boss2Missile)
				{
					if (abs(g_Boss2Missile[k].GetY() - g_MyShip.GetY()) < 30)
					{
						if (boss2MissileAtk)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyShip.GetX(),
								g_MyShip.GetY(), &g_Timer, 0, 50);
							
							if (!skillSceneOn)					
								g_MyShip.hp -= 3;
							
							if (g_MyShip.hp <= 0)
							{
								g_MyShip.Kill();
								_Play(11);
								MyShipDead();
							}
							boss2MissileAtk = false;
						}
					}
				}
			}
			g_Exploding[m].Draw(g_IpSecondarySurface);
		}
		for (m = 0; m < MAX_MISSILES; m++)
		{
			for (i = 0; i < MAX_BOSS; i++)
			{
				if (g_MyMissile[m].IsLive() && g_Boss1[i].IsLive() && m_bBoss == TRUE)
				{
					if (abs(g_MyMissile[m].GetX() - g_Boss1[i].GetX()) < 40)
						if (abs(g_MyMissile[m].GetY() - g_Boss1[i].GetY()) < 55)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(),
								g_MyMissile[m].GetY(), &g_Timer, 0, 50);
							g_MyMissile[m].Kill();
							g_Boss1[i].m_Enegy -= 1;

							++g_MyShip.mp;
							g_Exploding[m].Draw(g_IpSecondarySurface);

							if (g_Boss1[i].m_Enegy <= 0) //보스 체력이 0일때
							{
								g_Boss1[i].Kill();
								_Play(6);
								m_bBoss2 = TRUE;
								startMove = false;
								nBossFrame = 0;
								for (m = 0; m < MAX_BOSS_YMISSILES; m++)
								{
									for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
										g_Boss1Missile[m][m2].Kill();
									}
								}
							}
						}
				}
				else if (g_MyMissile[m].IsLive() && g_Boss2[i].IsLive() && m_bBoss2 == TRUE)
				{
					if (abs(g_MyMissile[m].GetX() - g_Boss2[i].GetX()) < 40)
						if (abs(g_MyMissile[m].GetY() - g_Boss2[i].GetY()) < 55)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(),
								g_MyMissile[m].GetY(), &g_Timer, 0, 50);
							g_MyMissile[m].Kill();
							g_Boss2[i].m_Enegy -= 1;
							++g_MyShip.mp;
							g_Exploding[m].Draw(g_IpSecondarySurface);

							if (g_Boss2[i].m_Enegy <= 0) //보스 체력이 0일때
							{
								g_Boss2[i].Kill();
								_Play(7);
								Boss2Kill = true;
								startMove = false;
								nBossFrame = 0;
								g_Boss3IntroTimer = g_Timer.time();
								for (m = 0; m < MAX_BOSS_YMISSILES; m++)
								{
									for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
										g_Boss1Missile[m][m2].Kill();
									}
								}
							}
						}
				}
				else if (g_MyMissile[m].IsLive() && g_Boss3[i].IsLive() && m_bBoss3 == TRUE)
				{
					if (abs(g_MyMissile[m].GetX() - g_Boss3[i].GetX()) < 40)
						if (abs(g_MyMissile[m].GetY() - g_Boss3[i].GetY()) < 55)
						{
							g_Exploding[m].Initialize(&g_ExplodingSprite, g_MyMissile[m].GetX(),
								g_MyMissile[m].GetY(), &g_Timer, 0, 50);
							g_MyMissile[m].Kill();
							g_Boss3[i].m_Enegy -= 1;
							++g_MyShip.mp;
							g_Exploding[m].Draw(g_IpSecondarySurface);

							if (g_Boss3[i].m_Enegy <= 0) //보스 체력이 0일때
							{
								EndingOn = true;
								g_Boss3[i].Kill();
								//엔딩
								MyShipWin();
								startMove = false;
								for (m = 0; m < MAX_BOSS_YMISSILES; m++)
								{
									for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
										g_Boss1Missile[m][m2].Kill();
									}
								}
							}
						}
				}
			}
		}

		for (m = 0; m < MAX_EXPLODES; m++)
		{
			for (i = 0; i < MAX_BOSS_YMISSILES; i++)
			{
				for (j = 0; j < MAX_BOSS_XMISSILES; j++)
				{
					if (g_Boss1Missile[i][j].IsLive() && g_MyShip.IsLive())
					{
						if (abs(g_Boss1Missile[i][j].GetX() - g_MyShip.GetX()) < 30)
							if (abs(g_Boss1Missile[i][j].GetY() - g_MyShip.GetY()) < 20)
							{
								g_Exploding[m].Initialize(&g_ExplodingSprite, g_Boss1Missile[i][j].GetX(),
									g_Boss1Missile[i][j].GetY(), &g_Timer, 0, 50);

								g_Boss1Missile[i][j].Kill();
								if (!skillSceneOn)
								{
									if (!barrierDraw)							
										g_MyShip.hp -= 1;				
								}
								if (g_MyShip.hp == 0)
								{
									g_MyShip.Kill();
									g_Exploding[m].Draw(g_IpSecondarySurface);
									_Play(11);
									MyShipDead();
								}								
							}
					}
				}
			}
		}

		for (m = 0; m < MAX_EXPLODES; m++)
		{
			for (i = 0; i < MAX_BOSS3_YMISSILES; i++)
			{
				for (j = 0; j < MAX_BOSS3_XMISSILES; j++)
				{
					if (g_MyShip.IsLive()&& boss3Missile)
					{
						if (abs(g_Boss3Missile[i][j].GetX() - g_MyShip.GetX()) < 40)
							if (abs(g_Boss3Missile[i][j].GetY() - g_MyShip.GetY()) < 50)			
							{
								g_Boss3Missile[i][j].Kill();
								if (boss3MissileAtk)
								{
									g_Exploding[m].Initialize(&g_ExplodingSprite, g_Boss3Missile[i][j].GetX(),
										g_Boss3Missile[i][j].GetY(), &g_Timer, 0, 50);


									if (!skillSceneOn)
									{
										if (!barrierDraw)						
											g_MyShip.hp -= 2;			
									}
									if (g_MyShip.hp <= 0)
									{
										g_MyShip.Kill();
										g_Exploding[m].Draw(g_IpSecondarySurface);
										_Play(11);
										MyShipDead();
									}
									boss3MissileAtk = false;
								}
							}
					}
				}
			}
		}

		for (i = 0; i < g_MyShip.hp; i++)
		{
			g_LifeSprite.Drawing(0, 120+50*i, 40, g_IpSecondarySurface, true);		
		}
		if (g_MyShip.hp > 5)
		{
			g_MyShip.hp = 5;
		}
		if (g_MyShip.mp > 8)
		{
			g_MyShip.mp = 8;
		}
		g_SkillLoadingBarSprite.Drawing(g_MyShip.mp, 180, 90, g_IpSecondarySurface, false);

		MaxPowerOn ? true : false;
		MaxPowerOn = !MaxPowerOn;
		if (skillSceneOn == false)
		{
			if (g_MyShip.mp == 8)
			{		
				if (!mpSoundOn)
				{
					_Play(5);
					mpSoundOn = true;
				}
				if(MaxPowerOn)
					g_MaxPowerSprite.Drawing(0, 320, 90, g_IpSecondarySurface, true); 
				if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_R))
				{
					if (skillSoundOn1)
					{
						_Play(8);
						skillSoundOn1 = true;
					}
					g_MyShip.Initialize(&g_MyShipSprite, 100, SCREEN_HEIGHT / 2, &g_Timer, 0, 25, 5);
					for (int i = 0; i < MAX_SECENE; i++)
					{
						g_SkillScene[i].Initialize(&g_SkillSceneSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &g_Timer, 0, 30);
						g_Skill[i].Initialize(&g_SkillSprite, g_MyShip.GetX() + 650, g_MyShip.GetY(), &g_Timer, 0, 170);
					}
					g_SkillTimeStart = g_Timer.time();
					skillSceneOn = true;
					g_MyShip.mp = 0;
					mpSoundOn = false;
				}
			}
		}

		if (skillSceneOn)
		{
			for (i = 0; i < MAX_SECENE; i++)
			{
				g_SkillScene[i].Draw(g_IpSecondarySurface);
			}

			if (g_Timer.elapsed(g_SkillTimeStart, 71*30))
			{
				skillSceneOn = false;
				skillAttackOn = true;
				skillSoundOn1 = false;
			}
			
		}

		if (skillAttackOn)
		{
			if (!skillSoundOn2)
			{
				_Play(9);
				_Play(10);
				skillSoundOn2 = true;
			}
			for (i = 0; i < MAX_SECENE; i++)
			{

				g_Skill[i].Drawskill(g_IpSecondarySurface);
				if (enemyCnt > 0)
				{
					for (i = 0; i < MAX_XENEMYS; i++)
					{
						for (j = 0; j < MAX_YENEMYS; j++)
						{
							g_Enemy[i][j].Kill();
						}
					}
					enemyCnt = 0;
				}

				if (g_Boss1[i].IsLive())
				{
					if (skillAtkEn)
					{
						if (abs(g_Skill[i].GetY() - g_Boss1[i].GetY()) < 9999)
							{
								if (!skillAtkBoss)
								{
									g_Boss1[i].m_Enegy -= 20;
									if (g_Boss1[i].m_Enegy <= 0) //보스 체력이 0일때
									{
										g_Boss1[i].Kill();
										_Play(6);
										m_bBoss2 = TRUE;
										startMove = false;
										skillAtkEn = false;
										for (m = 0; m < MAX_BOSS_YMISSILES; m++)
										{
											for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
												g_Boss1Missile[m][m2].Kill();
											}
										}
									}
									skillAtkBoss = true;
								}
							}
					}
				}
				else if (g_Boss2[i].IsLive())
				{
					if (skillAtkEn)
					{
							if (abs(g_Skill[i].GetY() - g_Boss2[i].GetY()) < 9999)
							{
								if (!skillAtkBoss2)
								{									
									g_Boss2[i].m_Enegy -= 20;									
									if (g_Boss2[i].m_Enegy <= 0) //보스 체력이 0일때
									{
										g_Boss2[i].Kill();
										_Play(7);
										Boss2Kill = true;
										startMove = false;
										skillAtkEn = false;
										g_Boss3IntroTimer = g_Timer.time();
										for (m = 0; m < MAX_BOSS_YMISSILES; m++)
										{
											for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
												g_Boss1Missile[m][m2].Kill();
											}
										}
									}
									skillAtkBoss2 = true;
									skillAtkEn = true;
								}
							}
					}
				}
				else if (g_Boss3[i].IsLive())
				{
					if (skillAtkEn == true)
					{
							if (abs(g_Skill[i].GetY() - g_Boss3[i].GetY()) < 9999)
							{
								if (skillAtkBoss2 == false)
								{
									g_Boss3[i].m_Enegy -= 20;
									if (g_Boss3[i].m_Enegy <= 0) //보스 체력이 0일때
									{
										EndingOn = true;
										g_Boss3[i].Kill();
										startMove = false;
										skillAtkEn = false;
										MyShipWin();//엔딩장면 실행
										for (m = 0; m < MAX_BOSS_YMISSILES; m++)
										{
											for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++) {
												g_Boss1Missile[m][m2].Kill();
											}
										}
										for (m = 0; m < MAX_BOSS3_YMISSILES; m++)
										{
											for (m2 = 0; m2 < MAX_BOSS3_XMISSILES; m2++) {
												g_Boss3Missile[m][m2].Kill();
											}
										}
									}
									skillAtkBoss2 = true;
									skillAtkEn = true;
								}
							}
					}
				}
			}
			if (g_Timer.elapsed(g_SkillTimeStart, 7*170))
			{
				skillAttackOn = false;
				skillSoundOn2 = false;
				skillAtkBoss2 = false;
			}
		}
		if (!Boss2Kill)
		{
			for (i = 0; i < MAX_SECENE; i++)
			{
				g_Boss3Intro[i].Initialize(&g_Boss3IntroSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &g_Timer, 0, 30);
			}
		}
		if (Boss2Kill)
		{
			SndObjStop(Sound[0]);
			if (!cellIntroSoundOn)
			{
				_Play(14);
				cellIntroSoundOn = true;
			}
			for (i = 0; i < MAX_SECENE; i++)
			{
				g_Boss3Intro[i].Draw(g_IpSecondarySurface);
			}
			if (g_Timer.elapsed(g_Boss3IntroTimer, 261 * 30))
			{
				m_bBoss3 = TRUE;
				Boss2Kill = false;
				boss3BgM = true;
				boss3BackGround = true;
			}
		}
		if (boss3BgM)
		{
			SndObjPlay(Sound[15], DSBPLAY_LOOPING);
		}
		if (m_bGameClear == FALSE)
		{
			for (i = 0; i < MAX_SECENE; i++)
			{
				g_Ending[i].Initialize(&g_EndingSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &g_Timer, 0, 30);
			}
		}
		if (m_bGameClear == TRUE)
		{
			SndObjStop(Sound[15]);
			if (!EndingSound)
			{
				_Play(16);
				EndingSound = true;
			}
			for (i = 0; i < MAX_SECENE; i++)
			{
				g_Ending[i].DrawEnding(g_IpSecondarySurface);
			}
			if (g_Timer.elapsed(g_EndingTimer, 208 * 60))
			{
				for (i = 0; i < MAX_SECENE; i++)
				{
					g_Ending[i].Kill();
				}
				m_bGameClear = FALSE;
				m_bGameFirst = TRUE;
				intro = true;
				GameMain();
				boss3BackGround = false;
				cellIntroSoundOn = false;
				introSoundOn = false;
			}
		}

		if (m_bGameDead == TRUE)
		{
			g_DeadSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, g_IpSecondarySurface, true);
			if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_Y))
			{
				m_bGameFirst = TRUE;
				GameMain();
				startMove = false;
				boss3BackGround = false;
				cellIntroSoundOn = false;
				_Play(12);
				SndObjStop(Sound[15]);
				for (i = 0; i < MAX_BOSS3_YMISSILES; i++)
				{
					for (j = 0; j < MAX_BOSS3_XMISSILES; j++)
					{
						g_Boss3Missile[i][j].Kill();
					}
				}
			}
			if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_N))
			{
				PostQuitMessage(0);
			}
		}

		HRESULT hRresult;
		if (FAILED(hRresult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))
		{
			if (hRresult == DDERR_SURFACELOST)
				g_IpPrimarySurface->Restore();
		}
	}
}

BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 IpKeyboard, int dikcode) {
	char KeyBuffer[256];
	HRESULT hResult;

	if (IpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer) == DIERR_INPUTLOST)
	{
		while (hResult = IpKeyboard->Acquire() == DIERR_INPUTLOST)
			hResult = IpKeyboard->Acquire();
		IpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer);
	}
	return(KeyBuffer[dikcode] & 0x80);
}
	


void InitGame() { //게임 초기화 기능(플레이어, 적 개수, 시작좌표, 체력, 이동속도, 발사체, 애니메이션, 아이템 등 설정) 
	int i, j, m, m2;

	for (i = 0; i < MAX_EXPLODES; i++)
		if (g_Exploding[i].IsLive()) 
			g_Exploding[i].Kill();

	if (g_MyShip.IsLive()) 
		g_MyShip.Kill();

	g_MyShip.Initialize(&g_MyShipSprite, 400, 550, &g_Timer, 0, 25, 5); //플레이어의 시작 좌표

	for (m = 0; m < MAX_MISSILES; m++)
	{
		if (g_MyMissile[m].IsLive()) 
			g_MyMissile[m].Kill();
	}

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
		{
			if (g_Enemy[i][j].IsLive())
				g_Enemy[i][j].Kill();
			if (g_EnemyMissile[i][j].IsLive()) 
				g_EnemyMissile[i][j].Kill();
		}

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
		{
			g_Enemy[i][j].Initialize(&g_EnemySprite, 800+(i + 1) * 100, (j + 1) * 60, &g_Timer, 0, 25, 5);//기본 적 생성
			g_Enemy[i][j].m_speedy = 2;
		}

	for (m = 0; m < MAX_BOSS_YMISSILES; m++)
	{
		for (m2 = 0; m2 < MAX_BOSS_XMISSILES; m2++)
		{
			if (g_Boss1Missile[m][m2].IsLive())
				g_Boss1Missile[m][m2].Kill();
		}
	}

	for (m = 0; m < MAX_BOSS3_YMISSILES; m++)
	{
		for (m2 = 0; m2 < MAX_BOSS3_XMISSILES; m2++)
		{
			if (g_Boss3Missile[m][m2].IsLive())
				g_Boss3Missile[m][m2].Kill();
		}
	}

	for (m = 0; m < MAX_ITEM; m++)//아이템 개수 설정
	{
		if (g_Item[m].IsLive())
			g_Item[m].Kill();
	}
	
	for (i = 0; i < MAX_BOSS; i++)//보스의 체력, 이동속도, 시작좌표 설정
	{
		if (g_Boss1[i].IsLive()) g_Boss1[i].Kill();
		g_Boss1[i].Initialize(&g_Boss1Sprite, (i + 1) * 1500, 300, &g_Timer, 0, 25, 5);
		g_Boss1[i].m_speedy = 2;
		g_Boss1[i].m_Enegy = 20;

		if (g_Boss2[i].IsLive()) g_Boss2[i].Kill();
		g_Boss2[i].Initialize(&g_Boss2Sprite, (i + 1) * 1500, 300, &g_Timer, 0, 25, 5);
		g_Boss2[i].m_speedy = 2;
		g_Boss2[i].m_Enegy = 30;

		if (g_Boss3[i].IsLive()) g_Boss3[i].Kill();
		g_Boss3[i].Initialize(&g_Boss3Sprite, (i + 1) * 1500, 300, &g_Timer, 0, 25, 5);
		g_Boss3[i].m_speedy = 2;
		g_Boss3[i].m_Enegy = 40;
	}
	
	for (i = 0; i < MAX_SECENE; i++)//애니메이션(BMP파일 차례로 보여줌) 설정
	{
		if (g_SkillScene[i].IsLive()) g_SkillScene[i].Kill();
		if (g_Skill[i].IsLive()) g_Skill[i].Kill();
		if (g_Boss2Missile[i].IsLive()) g_Boss2Missile[i].Kill();
		if (g_Boss3Intro[i].IsLive()) g_Boss3Intro[i].Kill();
		if (g_Ending[i].IsLive()) g_Ending[i].Kill();
	}
	
	EndingSound = false;//엔딩 사운드
	EndingOn = false;//엔딩 장면
	Boss2Kill = false;//두번째 보스가 죽었을 때
	boss3BgM = false;//보스 등장 사운드
	m_bGameDead = FALSE; //사망 이벤트
	m_bGameClear = FALSE;//클리어 이벤트
	m_bGameFirst = FALSE;//게임 시작
	m_bBoss = FALSE;//첫번째 보스
	m_bBoss2 = FALSE;//두번째 보스
	m_bBoss3 = FALSE;//세번째 보스
	enemyCnt = MAX_XENEMYS * MAX_YENEMYS;//기본 적
	SndObjPlay(Sound[0], DSBPLAY_LOOPING);  //인게임 브금
}

void Intro()
{		
	if (!introSoundOn)
	{
		_Play(2);
		introSoundOn = true;
	}
	g_IntroSprite.Drawing(0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, g_IpSecondarySurface, true);//타이틀 화면
	if (g_Timer.elapsed(g_Introtimestart, 100))//W or S 입력, elapsed 함수로 경과된 시간을 구해 그 시간 동안은 키 입력을 받지 않음.(빠르게 버튼 이동 방지)
	{
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_W) || (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_S)))
		{
			state += 1;
			_Play(3);
		}
	}
	//state값을 2로 나눈 나머지 값을 두개의 경우로 나눠서 시작 or 종료 버튼 선택
	if (state % 2 == 0)
	{
		g_GameStartSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 180, g_IpSecondarySurface, true);
		g_QuitSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240, g_IpSecondarySurface, true);
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_SPACE))
		{
			intro = false;
			SndObjStop(Sound[2]);
		}
	}
	else if (state % 2 == 1)
	{
		g_GameStartSprite.Drawing(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 180, g_IpSecondarySurface, true);
		g_QuitSprite.Drawing(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240, g_IpSecondarySurface, true);
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_SPACE))
			PostQuitMessage(0);	
	}		
	HRESULT hRresult;
	if (FAILED(hRresult = g_IpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))
		{
			if (hRresult == DDERR_SURFACELOST)
				g_IpPrimarySurface->Restore();
		}	
}

void MyShipDead() {
	m_bGameDead = TRUE;
}

void MyShipWin() {
	m_bGameClear = TRUE;
	if (EndingOn)
	{
		g_EndingTimer = g_Timer.time();
		EndingOn = false;
	}
}

void _Play(int num) {
	SndObjPlay(Sound[num], NULL);
}