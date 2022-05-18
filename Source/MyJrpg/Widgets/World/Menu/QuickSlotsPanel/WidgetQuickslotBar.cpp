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

void UWidgetQuickslotBar::CreateBeltSlots()
{
	USpacer* RightSpace = NewObject<USpacer>(this);
	
	UHorizontalBoxSlot* SlotWant =Cast<UHorizontalBoxSlot>( m_ScrollBarQuickSlot->AddChild(RightSpace) );
	
	SlotWant->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	
	int Num = (*m_InvenHolder)->GetAryItems().Num();

	for(int i=0; i < Num; i++)
	{
		UWidgetBaseElement* ItemEle = CreateWidget<UWidgetBaseElement>(this, m_ClassWidgetItemEle);

		ItemEle->SetBoxSize(110,110);

		m_AryQuickSlot.Add(ItemEle);
		
		m_ScrollBarQuickSlot->AddChild(m_AryQuickSlot[i]);

		m_AryQuickSlot[i]->SetPadding(FMargin(0,0,15,0));

		m_AryQuickSlot[i]->SetIndex(i);

		m_AryQuickSlot[i]->m_OnFocus.AddUObject(this,&UWidgetQuickslotBar::OnClick);

		m_AryQuickSlot[i]->SetDragable(false);

		m_AryQuickSlot[i]->SetHoldable(false);
	}

}

void UWidgetQuickslotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_InvenHolder = UMyLib::GetEquip()->GetBeltHolder();
	
	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetQuickslotBar::OnBeltEquipChanged);
	OnBeltEquipChanged();
}

void UWidgetQuickslotBar::NativeDestruct()
{
	Super::NativeDestruct();
	
	m_InvenHolder = nullptr;
}

void UWidgetQuickslotBar::OnClick(UWidgetBaseElement* ele)
{
	ele->SetMyUnFocus();
	
	int Index = ele->GetIndex();
	
	UInventory* Belt = *m_InvenHolder;

	FName ID = Belt->GetItemRef(Index).m_ID;

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
}
void UWidgetQuickslotBar::OnBeltEquipChanged()
{
	UInventory* Belt = *m_InvenHolder;
	if(!Belt)
	{
		Clear();
		return;
	}
	
	if(m_AryQuickSlot.Num() < 1)
	{
		CreateBeltSlots();
	
		(*m_InvenHolder)->m_OnInvenChanged.AddUObject(this, &UWidgetQuickslotBar::UpdateQuickSlots);
	}
	
	UpdateQuickSlots();
}

void UWidgetQuickslotBar::UpdateQuickSlots()
{
	int Index = 0;
	
	for(const FItemSpec& Item : (*m_InvenHolder)->GetAryItems())
	{
		if(Item.m_ID.IsNone())
		{
			m_AryQuickSlot[Index]->Clear();
			Index++;
			continue;
		}
		
		SetItem(m_AryQuickSlot[Index], Item);
		Index++;
	}
}

void UWidgetQuickslotBar::SetItem(UWidgetBaseElement* target, const FItemSpec& itemSpec)
{
	target->SetFocusable(true);
	
	const FItemDataRow& Data = UMyLib::GetItemData(itemSpec.m_ID);

	target->SetIcon(Data.m_Icon);
	
	target->SetGlowColor(Data.m_ColorHandle);

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

		target->ShowDurBar((float)itemSpec.m_nDurability / (float)Data.m_nDurability);
	}
	else
	{
		FString Str = FString::Printf(TEXT("%d"), itemSpec.m_nLvStack);
			
		target->SetTextStackLv(Str);

		target->HideDurBar();
	}
}

void UWidgetQuickslotBar::Clear()
{
	m_ScrollBarQuickSlot->ClearChildren();
	m_AryQuickSlot.Reset();
}
