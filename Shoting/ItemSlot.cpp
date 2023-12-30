#include"ItemSlot.h"
#include "define.h"

ItemSlot::ItemSlot()
	: m_ItemSpriteIndex(EMPTY_SLOT)
	, m_Item()
{}

void ItemSlot::Initialize(Sprite* pSprite, int x, int y)
{
	GameObject::Initialize(true, pSprite, x, y, 0, 0);
}

void ItemSlot::Draw(int frame, LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::DrawTargetFrame(m_X, m_Y, frame, lpSurface);
}

void ItemSlot::DrawItem(Sprite* pSprite, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	pSprite -> Drawing(m_ItemSpriteIndex, x, y, lpSurface, true);
}

void ItemSlot::UseItem()
{
	switch (m_ItemSpriteIndex)
	{
	case 0:
		m_Item.PlusHp(3);
		break;
	case 1:
		m_Item.PlusMp(9);
		break;
	case 2:
		m_Item.OnBarrior();
		break;
	default:
		break;
	}
}