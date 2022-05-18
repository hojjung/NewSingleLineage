// All right Reserve 2021 HereticByte


#include "WidgetEquipInvenPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/ItemDDO.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"

void UWidgetEquipInvenPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_InvenPanel->Init(UMyLib::GetPlayerInven());

	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocused);

	m_InvenPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm);

	m_BagPanel->m_OnFocus.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocused);

	m_BagPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm);

	m_BeltPanel->m_OnFocus.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocused);

	m_BeltPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm);

	UMyLib::GetEquip()->m_OnEquipChanged.AddUObject(this, &UWidgetEquipInvenPanel::OnEquipChanged);
}

void UWidgetEquipInvenPanel::OnEquipChanged()
{
	if(!UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_BagPanel->Clear();
	}
	else
	{
		m_BagPanel->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
		m_BagPanel->OpenPanel();

		m_DeleBag = UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().AddUObject(m_EquipPanel, &UWidgetEquipPanel::UpdateSlots);
	}

	if(!UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_BeltPanel->Clear();
	}
	else
	{
		m_BeltPanel->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
		m_BeltPanel->OpenPanel();

		m_DeleBelt = UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().AddUObject(m_EquipPanel, &UWidgetEquipPanel::UpdateSlots);
	}
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
		ele->SetTextFocus(NSLOCTEXT("UWidgetEquipInvenPanel","FocusEquip","장착?"));
		break;
	}
}

void UWidgetEquipInvenPanel::OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	const FItemSpec& Item = inven->GetItemConstRef(index);

	const FItemDataRow ItemData = UMyLib::GetItemData(Item.m_ID);
	
	switch (UMyLib::GetItemType(ItemData))
	{
	case EItemType::None:
	case EItemType::misc:
		ele->SetMyUnFocus();
		break;
	case EItemType::Consume:
		UMyGameInstance::Get->m_QuickManager->ExeItem(ItemData.m_ClassExeItem,inven,index,1);
		break;
	case EItemType::Equip:
		EEquipSlotType SlotT = UMyLib::GetEquipItemSlot(Item.m_ID);
		UMyLib::GetEquip()->Equip(SlotT, inven, index);
		inven->UpdateInventory();
		break;
	}
}

void UWidgetEquipInvenPanel::ClosePanel()
{
	Super::ClosePanel();

	m_EquipPanel->Close();
	
	m_InvenPanel->ClosePanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().Remove(m_DeleBag);
	}

	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().Remove(m_DeleBelt);
	}

	m_BagPanel->ClosePanel();

	m_BeltPanel->ClosePanel();
}

void UWidgetEquipInvenPanel::OpenInventory()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_EquipPanel->Open();

	m_InvenPanel->OpenPanel();

	m_BagPanel->OpenPanel();

	m_BeltPanel->OpenPanel();
}

UWidgetInventory* UWidgetEquipInvenPanel::GetInvenPanel()
{
	return m_InvenPanel;	
}

UWidgetEquipPanel* UWidgetEquipInvenPanel::GetEquipPanel()
{
	return m_EquipPanel;
}