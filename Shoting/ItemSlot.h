#pragma once
#include "GameObject.h"
#include"item.h"

class ItemSlot : public GameObject
{
public:
	ItemSlot();
	~ItemSlot() = default;
	int GetItemSpriteIndex() const;
	void SetItemSpriteIndex(int index);
	void Initialize(Sprite* pSprite, int x, int y);
	void Draw(int frame, LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawItem(Sprite* pSprite, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void UseItem();
private:
	Item m_Item;
	int m_ItemSpriteIndex;
};

inline int ItemSlot::GetItemSpriteIndex() const
{
	return m_ItemSpriteIndex;
}

inline void ItemSlot::SetItemSpriteIndex(int index)
{
	m_ItemSpriteIndex = index;
}