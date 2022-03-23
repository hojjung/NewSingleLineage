#include "WidgetEquipPanel.h"

#include "WidgetEquipInvenPanel.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetEquipPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryEquips.Reset();
	m_AryEquips.Add(m_Weapon);
	m_AryEquips.Add(m_Head);
	m_AryEquips.Add(m_Torso);
	m_AryEquips.Add(m_Hand);
	m_AryEquips.Add(m_Leg);
	m_AryEquips.Add(m_Ring);
	m_AryEquips.Add(m_Neckless);
	m_AryEquips.Add(m_EarRing);
	m_AryEquips.Add(m_Belt);

	m_Weapon->SetSlot(EEquipSlotType::Weapon);
	m_Head->SetSlot(EEquipSlotType::Head);
	m_Torso->SetSlot(EEquipSlotType::Torso);
	m_Hand->SetSlot(EEquipSlotType::Hand);
	m_Leg->SetSlot(EEquipSlotType::Leg);
	m_Ring->SetSlot(EEquipSlotType::Ring);
	m_Neckless->SetSlot(EEquipSlotType::Neckless);
	m_EarRing->SetSlot(EEquipSlotType::EarRing);
	m_Belt->SetSlot(EEquipSlotType::Belt);

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetEquipPanel::UpdateSlots);
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

void UWidgetEquipPanel::UpdateSlots()
{
	const TArray<const FName*>& EquippedItems = UMyGameInstance::Get->m_EquipManager->GetEquipAry();

	for (int i = 0; i < m_AryEquips.Num(); i++)
	{
		if (!EquippedItems[i])
		{
			m_AryEquips[i]->Clear();
			continue;
		}

		m_AryEquips[i]->UpdateElement();
	}
}

void UWidgetEquipPanel::RequestFilterItems()
{
	UMyLib::GetCanvas()->GetEquipInvenMenu()->GetInvenPanel()->OnFilterEquips();
}
