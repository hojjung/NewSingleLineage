#include "WidgetEquipPanel.h"

#include "WidgetEquipInvenPanel.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/PreviewActorManager.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetEquipPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Equip = UMyLib::GetEquip();

	m_Inven = UMyLib::GetPlayerInven();

	m_AryEquips.Reset();
	m_AryEquips.Add(nullptr);
	m_AryEquips.Add(m_Weapon);
	m_AryEquips.Add(m_Head);
	m_AryEquips.Add(m_Torso);
	m_AryEquips.Add(m_Hand);
	m_AryEquips.Add(m_Leg);
	m_AryEquips.Add(m_Ring);
	m_AryEquips.Add(m_Neckless);
	m_AryEquips.Add(m_EarRing);
	m_AryEquips.Add(m_Belt);
	m_AryEquips.Add(m_Bag);
	//
	int Iter = 0;
	for (UWidgetBaseElement* Ele : m_AryEquips)
	{
		if (!m_AryEquips[Iter])
		{
			Iter++;
			continue;
		}
		m_AryEquips[Iter]->SetIndex(Iter);

		m_AryEquips[Iter]->m_OnFocus.AddUObject(this, &UWidgetEquipPanel::OnFocused);

		m_AryEquips[Iter]->m_OnDrag.AddUObject(this, &UWidgetEquipPanel::OnDrag);

		m_AryEquips[Iter]->m_OnDrop.AddUObject(this, &UWidgetEquipPanel::OnDrop);

		Iter++;
	}

	m_Preview->Init(UMyGameInstance::Get->m_PreviewActorManager);
}

FReply UWidgetEquipPanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

FReply UWidgetEquipPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

void UWidgetEquipPanel::Open()
{
	m_Handle = m_Equip->m_OnEquipChanged.AddUObject(this, &UWidgetEquipPanel::UpdateSlots);
	UpdateSlots();

	UMyGameInstance::Get->m_PreviewActorManager->ShowPawn();
}

void UWidgetEquipPanel::Close()
{
	m_Equip->m_OnEquipChanged.Remove(m_Handle);

	UMyGameInstance::Get->m_PreviewActorManager->HidePawn();
}

void UWidgetEquipPanel::UnFocusCurrent()
{
	if(m_CurrentFocused.Get())
	{
		m_CurrentFocused.Get()->SetMyUnFocus();

		m_CurrentFocused= nullptr;
	}
}

void UWidgetEquipPanel::UpdateSlots()
{
	const FItemSpec* EquippedItems = m_Equip->GetEquipAry();
	
	for (int i = 0; i < m_AryEquips.Num(); i++)
	{
		if (EquippedItems[i].m_ID.IsNone())
		{
			if(m_AryEquips[i])
				m_AryEquips[i]->Clear();
			
			continue;
		}
		UpdateElement(m_AryEquips[i], EquippedItems[i]);
	}

	UnFocusCurrent();
}

void UWidgetEquipPanel::UpdateElement(UWidgetBaseElement* ele, const FItemSpec& item)
{
	ele->SetHoldable(true);

	if (ele == m_Belt)
	{
		bool b =  UMyLib::GetEquip()->IsBeltUnequipable();
		
		ele->SetFocusable(b);
		ele->SetDragable(b);
	}
	else if (ele == m_Bag)
	{
		bool b =  UMyLib::GetEquip()->IsBagUnequipable();
		
		ele->SetFocusable(b);
		ele->SetDragable(b);
	}
	else
	{
		ele->SetFocusable(true);
		ele->SetDragable(true);
	}
	
	const FItemDataRow& Data = UMyLib::GetItemData(item.m_ID);

	ele->SetIcon(Data.m_Icon);

	ele->SetGlowColor(Data.m_ColorHandle);

	if (item.m_nLvStack > 0)
	{
		FString Str = FString::Printf(TEXT("+%d"), item.m_nLvStack);

		ele->SetTextStackLv(Str);
	}
	else
	{
		ele->HideTextStackLv();
	}
}

bool UWidgetEquipPanel::TryUnequip(EEquipSlotType t)
{
	UInventory* Inven = UMyLib::GetPlayerInven();
	
	if(!m_Equip->Unequip(t,Inven))
	{
		Inven = UMyLib::GetEquip()->GetBag();
		
		if(!m_Equip->Unequip(t,Inven))
		{
			Inven = UMyLib::GetEquip()->GetBelt();
		
			if(!m_Equip->Unequip(t,Inven))
			{
				return false;
			}
		}	
	}
	
	UnFocusCurrent();
	
	Inven->UpdateInventory();

	return true;
}
void UWidgetEquipPanel::OnFocused(UWidgetBaseElement* ele)
{
	if(m_CurrentFocused.Get() && m_CurrentFocused.Get() == ele)
	{
		EEquipSlotType t =  (EEquipSlotType)ele->GetIndex();
		
		TryUnequip(t);
		
		return;
	}
	UnFocusCurrent();
	
	m_CurrentFocused = ele;

	m_CurrentFocused->SetTextFocus(NSLOCTEXT("UWidgetEquipPanel","FocusUnequip","해제?"));
}

void UWidgetEquipPanel::OnDrag(UWidgetBaseElement* ele)
{
	UnFocusCurrent();

	UItemDDO::GetDDOInst->m_FromEquip = m_Equip;

	UItemDDO::GetDDOInst->m_nIndex = ele->GetIndex();
}

void UWidgetEquipPanel::OnDrop(UWidgetBaseElement* ele)
{
	UnFocusCurrent();

	const FItemSpec& Item = UItemDDO::GetDDOInst->GetItem();

	const FItemDataRow& ItemData = UMyLib::GetItemData(Item.m_ID);

	EEquipSlotType t =  (EEquipSlotType)ele->GetIndex();

	
	if(!UMyLib::IsEquip(ItemData) || ItemData.m_ItemType != t)
	{
		return;
	}
	
	if(t == EEquipSlotType::Bag)
	{
		if(!UMyLib::GetEquip()->IsBagUnequipable())
		{
			return;
		}
	}
	else if(t == EEquipSlotType::Belt)
	{
		if(!UMyLib::GetEquip()->IsBeltUnequipable())
		{
			return;
		}
	}

	m_Equip->Equip(t,UItemDDO::GetDDOInst->m_FromInven.Get(),UItemDDO::GetDDOInst->m_nIndex);

	UItemDDO::GetDDOInst->m_FromInven->UpdateInventory();
}