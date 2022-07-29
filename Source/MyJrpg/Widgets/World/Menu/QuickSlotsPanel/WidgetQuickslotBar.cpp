#include "WidgetQuickslotBar.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetQuickslotBar::SetQuickSlot()
{
	m_AryQuickSlot.Reset(4);
	m_AryQuickSlot.Add(m_Quick01);
	m_AryQuickSlot.Add(m_Quick02);
	m_AryQuickSlot.Add(m_Quick03);
	m_AryQuickSlot.Add(m_Quick04);

	for(int i=0; i < 4; i++)
	{
		m_AryQuickSlot[i]->SetIndex(i);

		m_AryQuickSlot[i]->m_OnClick.AddUObject(this,&UWidgetQuickslotBar::OnClick);
	}
}

void UWidgetQuickslotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetQuickSlot();
	
	m_InvenHolder = UMyLib::GetPlayerInven();
	
	m_InvenHolder->m_OnInvenChanged.AddUObject(this, &UWidgetQuickslotBar::OnInvenChanged);
	
	OnInvenChanged();
}

void UWidgetQuickslotBar::OnClick(UWidgetQuickSlotEle* ele)
{
	int Index = ele->GetIndex();
	
	UInventory* Belt = m_InvenHolder.Get();

	FName ID = Belt->GetItemRef(Index).m_ID;

	if(ID.IsNone())
	{
		return;
	}

	const FItemDataRow& ItemData = UMyLib::GetItemData(ID);

	EItemType Type = UMyLib::GetItemType(ItemData);
	
	switch (Type)
	{
	case EItemType::Consume:
		UMyGameInstance::Get->m_QuickManager->ExeItem(ItemData.m_ClassExeItem,Belt,Index,1);
		break;
	case EItemType::Equip:
		UMyLib::GetEquip()->Equip(ItemData.m_ItemType,Belt,Index);
		break;
	default:
		break;
	}
	
	OnInvenChanged();
}

void UWidgetQuickslotBar::OnInvenChanged()
{
	int Index = -1;

	while (++Index < 4)
	{
		const FItemSpec& Item = m_InvenHolder->GetItemConstRef(Index);

		if(Item.m_ID.IsNone())
		{
			m_AryQuickSlot[Index]->Clear();
			continue;
		}
		SetItem(m_AryQuickSlot[Index], Item);
	}
}

void UWidgetQuickslotBar::SetItem(UWidgetQuickSlotEle* target, const FItemSpec& itemSpec)
{
	const FItemDataRow& Data = UMyLib::GetItemData(itemSpec.m_ID);

	target->SetIcon(Data.m_Icon);
	
	bool IsEquip = UMyLib::IsEquip(Data);

	if(IsEquip)
	{
		if(itemSpec.m_nLvStack > 0)
		{
			FString Str = FString::Printf(TEXT("+%d"), itemSpec.m_nLvStack);
			
			target->SetTextStackLv(Str);
		}
		else
		{
			target->HideTextStackLv();
		}
	}
	else
	{
		FString Str = FString::Printf(TEXT("%d"), itemSpec.m_nLvStack);
			
		target->SetTextStackLv(Str);
	}
}