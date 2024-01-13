#include "Player.h"
#include <dinput.h>
#include "define.h"
#include "PlayerMissile.h"
#include "Dsutil.h"
#include "ItemSlot.h"
#include "Animation.h"

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTINPUTDEVICE8 g_IpDirectInputKeyboard;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern PlayerMissile playerMissile[MAX_PLAYER_MISSILES];
extern ItemSlot itemSlots[MAX_ITEM_SLOT];
extern HSNDOBJ Sound[17];

extern Animation g_SkillScene;

extern bool	isGameDead;

Player::Player()
	: m_MaxHp(5)
	, m_MaxMp(PLAYER_MAX_MP)
	, m_Mp(0)
	, m_LastMoveTime(Timer::Now())
	, m_LastAttackTime(Timer::Now())
	, m_MoveInterval(0)
	, m_IsAttacking(false)
	, m_IsBarrier(false)
	, m_IsChangeAttack(false)
{
}

void Player::Initialize(Sprite* pSprite, int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	GameObject::Initialize(true, pSprite, x, y, currentFrame, frameInterval);
	m_Hp = m_MaxHp;
	m_MoveInterval = moveInterval;
}

void Player::SetMp(int mp)
{
	m_Mp = mp;
}

void Player::PlusMp(int value)
{
	if (m_Mp == m_MaxMp)
		return;
		
	m_Mp += value;
	if (m_Mp >= m_MaxMp)
	{
		SndObjPlay(Sound[5], NULL);
		m_Mp = m_MaxMp;
	}
}

void Player::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::DrawTargetFrame(m_X, m_Y, m_CurrentFrame, lpSurface);
}

bool Player::CanMove()
{
	if (!m_IsLive)
		return false;
	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
		return true;

	return false;
}

void Player::Attack()
{
	int i;
	for (i = 0; i < MAX_PLAYER_MISSILES; i++)
	{
		if (!playerMissile[i].IsLive())
			break;
	}
	m_IsAttacking = true;
	m_CurrentFrame = m_IsChangeAttack ? 5 : 6;
	playerMissile[i].Revive();
	playerMissile[i].SetXY(m_X, m_Y - 20);
	m_IsChangeAttack = !m_IsChangeAttack;
	SndObjPlay(Sound[1], NULL);
}

bool Player::CheckBarrierTime()
{
	if (Timer::Elapsed(m_BarrierTime, 5000))
	{
		return true;
	}

	return false;
}

void Player::TakeDamage(int value)
{
	if (m_IsBarrier)
		return;

	m_Hp -= value;
	if (m_Hp <= 0)
	{
		isGameDead = true;
		Kill();
		SndObjPlay(Sound[11], NULL);
	}
}

bool Player::CanAttack()
{
	if (!m_IsLive)
		return false;

	if (Timer::Elapsed(m_LastAttackTime , MY_MISSILE_INTRVAL))
	{
		return true;
	}

	return false;
}

bool Player::CanSkill()
{
	return m_Mp == m_MaxMp;
}

void Player::Skill()
{
	m_IsAttacking = false;
	SetMp(0);
	SndObjPlay(Sound[8], NULL);
	SetXY(100, SCREEN_HEIGHT / 2);
	g_SkillScene.InitCurFrame(0);
	g_SkillScene.Revive();
}

void Player::InputKey()
{
	if(!m_IsAttacking) 
		m_CurrentFrame = 0;

	if (CanMove())
	{
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_A))
		{
			Left();
		}

		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_D))
		{
			Right();
		}

		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_W))
		{
			Up();
		}

		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_S))
		{
			Down();
		}
	}

	if (CanAttack())
	{
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_SPACE))
		{
			Attack();
		}
	}

	if (CanSkill())
	{
		if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_R))
		{
			Skill();
		}
	}

	if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_1))
	{
		itemSlots[0].UseItem();
	}

	if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_2))
	{
		itemSlots[1].UseItem();
	}

	if (DirectInputKeyboardDown(g_IpDirectInputKeyboard, DIK_3))
	{
		itemSlots[2].UseItem();
	}
}

