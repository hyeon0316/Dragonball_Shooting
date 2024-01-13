#include <time.h>
#include "Item.h"
#include "Player.h"
#include "Dsutil.h"
#include "ItemSlot.h"

extern Player player;
extern LPDIRECTDRAWSURFACE7 g_IpSecondarySurface;
extern HSNDOBJ Sound[17];
extern ItemSlot itemSlots[MAX_ITEM_SLOT];
int barrierTime = 0;

Item::Item()
	: m_SpeedX(4)
	, m_SpeedY(4)
	, m_MoveInterval(0)
	, m_LastMoveTime(Timer::Now())
{}

void Item::Initialize(Sprite* pSprite, int x, int y, int frameInterval, int moveInterval)
{
	GameObject::Initialize(false, pSprite, x, y, m_CurrentFrame, frameInterval);
	m_MoveInterval = moveInterval;
}

void Item::CheckArea()
{
	if (abs(m_Y - player.GetY()) < 30)
	{
		if (abs(m_X - player.GetX()) < 30)
		{
			for (int i = 0; i < MAX_ITEM_SLOT; i++)
			{
				if (itemSlots[i].GetItemSpriteIndex() != EMPTY_SLOT)
					continue;

				itemSlots[i].SetItemSpriteIndex(m_CurrentFrame);
				Kill();
				break;
			}
		}
	}
}

void Item::Move()
{
	if (!m_IsLive)
		return;

	if (Timer::Elapsed(m_LastMoveTime, m_MoveInterval))
	{	
		CheckArea();
		if (m_X > 1366)
		{
			m_X = 1366;
			m_SpeedX = rand() % 8 - 8;
			m_SpeedY = rand() % 8 - 4;
		}
		if (m_X < 0)
		{
			m_X = 0;
			m_SpeedX = rand() % 4 + 1;
			m_SpeedY = rand() % 8 - 4;
		}
		if (m_Y > 768)
		{
			m_Y = 768;
			m_SpeedX = rand() % 8 - 4;
			m_SpeedY = rand() % 8 - 8;
		}
		if (m_Y < 0)
		{
			m_Y = 0;
			m_SpeedX = rand() % 8 - 4;
			m_SpeedY = rand() % 4 + 1;
		}
		m_X += m_SpeedX;
		m_Y += m_SpeedY;
	}
}

void Item::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::DrawTargetFrame(m_X, m_Y, m_CurrentFrame, lpSurface);
}

void Item::PlusHp(int plusHp)
{
	int result = plusHp + player.GetHp();

	player.SetHp(result > player.GetMaxHp() ? player.GetMaxHp() : result);
	SndObjPlay(Sound[13], NULL);
}

void Item::PlusMp(int plusMp)
{
	player.PlusMp(plusMp);
}

void Item::OnBarrior()
{
	player.SetBarrier(true);
	SndObjPlay(Sound[4], NULL);
}
