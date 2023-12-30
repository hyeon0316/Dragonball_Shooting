#pragma once
#include<ddraw.h>
#include<dinput.h>
#include<stdio.h>
#include<Windows.h>
#include <time.h>
#include<dsound.h>
#include<vector>
#include"CBMP.h"
#include"Timer.h"
#include"Sprite.h"
#include"Enemy.h"
#include"PlayerMissile.h"
#include"EnemyMissile.h"
#include"Item.h"
#include "CAnimation.h"
#include"Dsutil.h"
#include"define.h"
#include "Player.h"
#include"ItemSlot.h"
#include "NormalEnemy.h"
#include "FirstBoss.h"
#include "SecondBoss.h"
#include "LastBoss.h"
#include "BossMissile.h"
#include "LastBossMissile.h"
#include "StageManager.h"

void InitGame();
void ClearEnemy();

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 IpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_IpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern LPDIRECTINPUT g_IpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard;

extern Sprite g_Boss3MissileSprite;
extern Sprite g_EndingSprite;
extern Sprite g_Boss3IntroSprite;
extern Sprite g_BackgroundSprite;
extern Sprite g_MyShipSprite;
extern Sprite playerMissileSprite;
extern Sprite g_ExplodingSprite;
extern Sprite g_ClearSprite;
extern Sprite g_DeadSprite;
extern Sprite g_IntroSprite;
extern Sprite g_GameStartSprite;
extern Sprite g_QuitSprite;
extern Sprite g_PlayerIconSprite;
extern Sprite g_BossIconSprite;
extern Sprite g_LifeSprite;
extern Sprite g_ItemSlotSprite;
extern Sprite g_ItemSprite;
extern Sprite g_BarrierEffectSprite;
extern Sprite g_Boss1Sprite;
extern Sprite g_EnemyMissileSprite;
extern Sprite g_Boss1MissileSprite;
extern Sprite g_EnemySprite;
extern Sprite g_Boss2MissileSprite;
extern Sprite g_SkillSceneSprite;
extern Sprite g_SkillSprite;
extern Sprite g_SkillLoadingBarSprite;
extern Sprite g_MaxPowerSprite;
extern Sprite g_Boss3Sprite;
extern Sprite g_Boss2Sprite;
extern HSNDOBJ Sound[17];

BossMissile bossMissiles[MAX_BOSS_XMISSILES][MAX_BOSS_YMISSILES];
Item dropItem;
Player player;
PlayerMissile playerMissile[MAX_PLAYER_MISSILES];
ItemSlot itemSlots[MAX_ITEM_SLOT];
EnemyMissile enemyMissiles[MAX_ENEMY_MISSILES];
LastBossMissile lastBossMissiles[MAX_LASTBOSS_XMISSILES];
CAnimation explosionEffects[MAX_EXPLODES];
CAnimation bossSecondAttack;
CAnimation g_SkillScene;
CAnimation g_Skill;
CAnimation g_Boss3Intro;
CAnimation g_Ending;
StageManager stageManager;

bool isGameFirst = false;
bool	isGameDead = false;
bool isHitReady = false;

vector<Enemy*> enemies;

void GameMain() {

	if (isGameFirst) InitGame();

	int i, j, k;

	player.InputKey();
	player.Draw(g_IpSecondarySurface);

	for (i = 0; i < MAX_PLAYER_MISSILES; i++)
	{
		playerMissile[i].Move();
		playerMissile[i].Draw(g_IpSecondarySurface);
	}

	if (player.CanSkill())
	{
		g_MaxPowerSprite.Drawing(0, 320, 90, g_IpSecondarySurface, true);
	}
	else
	{
		g_SkillLoadingBarSprite.Drawing(player.GetMp(), 180, 90, g_IpSecondarySurface, false);
	}
	g_PlayerIconSprite.Drawing(0, 45, 40, g_IpSecondarySurface, false);

	for (i = 0; i < player.GetHp(); i++)
	{
		g_LifeSprite.Drawing(0, 120 + 50 * i, 40, g_IpSecondarySurface, true);
	}

	if (stageManager.IsFirstBoss())
	{
		g_BossIconSprite.Drawing(0, 1310, 35, g_IpSecondarySurface, true);
	}

	if (stageManager.IsSecondBoss())
	{
		g_BossIconSprite.Drawing(1, 1310, 35, g_IpSecondarySurface, true);
	}

	if (stageManager.IsLastBoss())
	{
		g_BackgroundSprite.Drawing2(1, 400, 300, g_IpSecondarySurface, false);
		g_BossIconSprite.Drawing(2, 1310, 35, g_IpSecondarySurface, true);
	}
	else
	{
		g_BackgroundSprite.Drawing2(0, 400, 300, g_IpSecondarySurface, false);
	}

	if (player.GetBarrierStatus())
	{
		g_BarrierEffectSprite.Drawing(0, player.GetX() - 10, player.GetY() - 10, g_IpSecondarySurface, true);
		if (player.CheckBarrierTime())
		{
			player.SetBarrier(false);
		}
	}

	for (i = 0; i < MAX_ITEM_SLOT; i++)
	{
		itemSlots[i].Draw(0, g_IpSecondarySurface);
	}

	for (i = 0; i < enemies.size(); i++)
	{
		enemies[i]->ExecutePattern();
		enemies[i]->Draw(g_IpSecondarySurface);
	}

	for (i = 0; i < MAX_ENEMY_MISSILES; i++)
	{
		enemyMissiles[i].Draw(g_IpSecondarySurface);
		enemyMissiles[i].Move();
	}

	bossSecondAttack.DrawFrame(0, g_IpSecondarySurface);
	if (bossSecondAttack.IsLive() && player.IsLive())
	{
		if (isHitReady)
		{
			if (abs(bossSecondAttack.GetY() - player.GetY()) < 30)
			{
				player.TakeDamage(3);
				isHitReady = false;
			}
		}
	}

	if (g_Skill.IsLive())
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->IsLive())
			{
				enemies[i]->TakeDamage(30);
			}
		}
	}

	for (i = 0; i < MAX_BOSS_XMISSILES; i++)
	{
		for (j = 0; j < MAX_BOSS_YMISSILES; j++) {
			bossMissiles[i][j].Move();
			bossMissiles[i][j].Draw(g_IpSecondarySurface);
		}
	}

	for (i = 0; i < MAX_LASTBOSS_XMISSILES; i++)
	{
		lastBossMissiles[i].Move();
		lastBossMissiles[i].Draw(g_IpSecondarySurface);
	}

	dropItem.Move();
	dropItem.Draw(g_IpSecondarySurface);

	for (int i = 0; i < MAX_ITEM_SLOT; i++)
	{
		if (itemSlots[i].GetItemSpriteIndex() != EMPTY_SLOT) 
		{
			itemSlots[i].DrawItem(&g_ItemSprite, 45 + i * 70, 720, g_IpSecondarySurface);
		}
	}

	for (i = 0; i < MAX_EXPLODES; i++)
	{
		explosionEffects[i].Draw(g_IpSecondarySurface);
	}

	if (isGameDead)
	{
		g_DeadSprite.Drawing(0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, g_IpSecondarySurface, true);
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_Y))
		{
			isGameFirst = TRUE;
			GameMain();
			SndObjPlay(Sound[12], NULL);
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
	
/// <summary>
/// 게임 초기화 기능(플레이어, 적 개수, 시작좌표, 체력, 이동속도, 발사체, 애니메이션, 아이템 등 설정)
/// </summary>
void InitGame() {
	int i, j;

	stageManager.Init();
	for (i = 0; i < MAX_EXPLODES; i++)
	{
		if (explosionEffects[i].IsLive())
			explosionEffects[i].Kill();
		explosionEffects[i].Initialize(false, &g_ExplodingSprite, 0, 0, 0, 50);
	}

	if (player.IsLive())
		player.Kill();
	player.Initialize(&g_MyShipSprite, 400, 550, 0, 25, 5);

	for (i = 0; i < MAX_PLAYER_MISSILES; i++)
	{
		if (playerMissile[i].IsLive())
			playerMissile[i].Kill();
		playerMissile[i].Initialize(&playerMissileSprite, player.GetX(), player.GetY() - 20, 0, 10, 5);
	}

	if (dropItem.IsLive())
		dropItem.Kill();
	dropItem.Initialize(&g_ItemSprite, 0, 0, 50, 5);

	if (g_SkillScene.IsLive())
		g_SkillScene.Kill();
	g_SkillScene.Initialize(false, &g_SkillSceneSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 30);

	if (g_Skill.IsLive())
		g_Skill.Kill();
	g_Skill.Initialize(false, &g_SkillSprite, player.GetX() + 650, player.GetY(), 0, 170);

	if (bossSecondAttack.IsLive()) 
		bossSecondAttack.Kill(); 
	bossSecondAttack.Initialize(false, &g_Boss2MissileSprite, 0, 0, 0, 50);

	if (g_Boss3Intro.IsLive()) 
		g_Boss3Intro.Kill();
	g_Boss3Intro.Initialize(false, &g_Boss3IntroSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 30);

	if (g_Ending.IsLive()) 
		g_Ending.Kill();
	g_Ending.Initialize(false, &g_EndingSprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 30);

	for (i = 0; i < MAX_XENEMYS; i++)
	{
		for (j = 0; j < MAX_YENEMYS; j++)
		{
			Enemy* enemy = new NormalEnemy();
			enemy->Initialize(true, &g_EnemySprite, 800 + (i + 1) * 100, (j + 1) * 60, 0, 25, 5);
			enemies.push_back(enemy);
		}
	}

	Enemy* firstBoss = new FirstBoss();
	firstBoss->Initialize(false, &g_Boss1Sprite, 1500, 300, 0, 25, 5);
	enemies.push_back(firstBoss);

	Enemy* secondBoss = new SecondBoss();
	secondBoss->Initialize(false, &g_Boss2Sprite, 1500, 300, 0, 25, 5);
	enemies.push_back(secondBoss);

	Enemy* lastBoss = new LastBoss();
	lastBoss->Initialize(false, &g_Boss3Sprite, 1500, 300, 0, 25, 5);
	enemies.push_back(lastBoss);

	for (i = 0; i < MAX_ENEMY_MISSILES; i++)
	{
		if (enemyMissiles[i].IsLive())
			enemyMissiles[i].Kill();
		enemyMissiles[i].Initialize(&g_EnemyMissileSprite, 0, 0, 0, 150, 5);
	}

	for (i = 0; i < MAX_BOSS_XMISSILES; i++)
	{
		for (j = 0; j < MAX_BOSS_YMISSILES; j++)
		{
			if (bossMissiles[i][j].IsLive())
				bossMissiles[i][j].Kill();

			bossMissiles[i][j].Initialize(&g_Boss1MissileSprite, 0, 0, 0, 10, 5);
		}
	}

	for (i = 0; i < MAX_LASTBOSS_XMISSILES; i++)
	{
		if (lastBossMissiles[i].IsLive())
			lastBossMissiles[i].Kill();

		lastBossMissiles[i].Initialize(&g_Boss3MissileSprite, 0, 0, 0, 50, 5);
	}

	for (int i = 0; i < MAX_ITEM_SLOT; i++)
	{
		if (itemSlots[i].IsLive())
			itemSlots[i].Kill();

		itemSlots[i].Initialize(&g_ItemSlotSprite, 45 + 70 * i, 720);
	}

	stageManager.SetFirstBoss(false);
	stageManager.SetSecondBoss(false);
	stageManager.SetLastBoss(false);

	isGameDead = false; 
	isGameFirst = false;
	SndObjPlay(Sound[0], DSBPLAY_LOOPING);  
}

void ClearEnemy()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
	}
	enemies.clear();
}


