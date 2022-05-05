// All right Reserve 2021 HereticByte


#include "WidgetEquipInvenPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetEquipInvenPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_InvenPanel->Init(UMyLib::GetPlayerInven());

	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocused);

	m_InvenPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm);
}

void UWidgetEquipInvenPanel::OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	const FItemSpec& Item = inven->GetItemConstRef(index);

	switch (UMyLib::GetItemType(Item.m_ID))
	{
	case EItemType::None:
	case EItemType::misc:
		ele->SetMyUnFocus();
		break;
	case EItemType::Consume:
		ele->SetTextFocus(NSLOCTEXT("UWidgetEquipInvenPanel","FocusUse","사용?"));
		break;
	case EItemType::Equip:
		if(UMyLib::GetEquip()->IsItemEquipped(Item))
			ele->SetTextFocus(NSLOCTEXT("UWidgetEquipInvenPanel","FocusUnequip","해제?"));
		else
			ele->SetTextFocus(NSLOCTEXT("UWidgetEquipInvenPanel","FocusEquip","장착?"));
		break;
	}
}

void UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	const FItemSpec& Item = inven->GetItemConstRef(index);

	switch (UMyLib::GetItemType(Item.m_ID))
	{
	case EItemType::None:
	case EItemType::misc:
		ele->SetMyUnFocus();
		break;
	case EItemType::Consume:
		//use
		break;
	case EItemType::Equip:
		EEquipSlotType SlotT = UMyLib::GetEquipItemSlot(Item.m_ID);
		if(UMyLib::GetEquip()->IsItemEquipped(SlotT, Item))
		{
			UMyLib::GetEquip()->Unequip(SlotT);
		}
		else
		{
			UMyLib::GetEquip()->Equip(SlotT, index);
		}
		break;
	}
}

void UWidgetEquipInvenPanel::ClosePanel()
{
	Super::ClosePanel();
	
	m_InvenPanel->ClosePanel();
}

void UWidgetEquipInvenPanel::OpenInventory()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_InvenPanel->OpenPanel();
}

UWidgetInventory* UWidgetEquipInvenPanel::GetInvenPanel()
{
	return m_InvenPanel;	
}

UWidgetEquipPanel* UWidgetEquipInvenPanel::GetEquipPanel()
{
	return m_EquipPanel;
}

UWidgetStatPanel* UWidgetEquipInvenPanel::GetStatPanel()
{
	return m_StatPanel;
}
