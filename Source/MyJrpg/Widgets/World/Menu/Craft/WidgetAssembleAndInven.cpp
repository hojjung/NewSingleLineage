#include "WidgetAssembleAndInven.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetAssembleAndInven::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_InvenPanel->Init(UMyLib::GetPlayerInven());
	
	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetAssembleAndInven::OnPlInvenFocused);

	m_InvenPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetAssembleAndInven::OnPlInvenFocuseConfirm);

	m_BagPanel->m_OnFocus.AddUObject(this, &UWidgetAssembleAndInven::OnPlInvenFocused);

	m_BagPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetAssembleAndInven::OnPlInvenFocuseConfirm);

	m_AssemblePanel->m_OnComplete.BindUObject(this, &UWidgetAssembleAndInven::ClosePanel);
}

void UWidgetAssembleAndInven::OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	ele->SetTextFocus(NSLOCTEXT("UWidgetAssembleAndInven","Put","넣기?"));
}

void UWidgetAssembleAndInven::OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	//넣을곳이 없음
	//or 드래그드랍과 같은 함수
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

void UWidgetAssembleAndInven::ClosePanel()
{
	Super::ClosePanel();

	m_AssemblePanel->ClosePanel();
	
	m_InvenPanel->ClosePanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().Remove(m_DeleBag);
	}

	m_BagPanel->ClosePanel();
}

void UWidgetAssembleAndInven::ShowAssemble(UAssembleInst* inst)
{
	m_AssemblePanel->ShowAssemble(inst);
	
	OpenPanel();
}

void UWidgetAssembleAndInven::OpenPanel()
{
	Super::OpenPanel();

	m_InvenPanel->OpenPanel();

	m_BagPanel->OpenPanel();
}

UWidgetInventory* UWidgetAssembleAndInven::GetInvenPanel()
{
	return m_InvenPanel;	
}

UWidgetAssemble* UWidgetAssembleAndInven::GetAssemble()
{
	return m_AssemblePanel;
}