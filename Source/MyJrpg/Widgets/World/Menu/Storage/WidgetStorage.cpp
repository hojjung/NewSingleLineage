#include "WidgetStorage.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	m_StoragePanel->m_OnFocus.AddUObject(this, &UWidgetStorage::OnStorageInvenFocused);

	m_StoragePanel->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnStorageFocuseConfirm);
	
	m_InvenPanel->Init(UMyLib::GetPlayerInven());
	
	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetStorage::OnPlInvenFocused);

	m_InvenPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnPlInvenFocuseConfirm);

	m_Bag->ClosePanel();

	m_Belt->ClosePanel();
	
	m_Bag->m_OnFocus.AddUObject(this, &UWidgetStorage::OnPlInvenFocused);

	m_Bag->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnPlInvenFocuseConfirm);

	m_Belt->m_OnFocus.AddUObject(this, &UWidgetStorage::OnPlInvenFocused);

	m_Belt->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnPlInvenFocuseConfirm);
}

void UWidgetStorage::SetTargetInven(UInventory* storage)
{
	m_StoragePanel->Init(storage);
	
	m_StoragePanel->OpenPanel();
	
	m_InvenPanel->OpenPanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Bag->Init(UMyGameInstance::Get->m_EquipManager->GetBag());
		m_Bag->OpenPanel();
	}

	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_Belt->Init(UMyGameInstance::Get->m_EquipManager->GetBelt());
		m_Belt->OpenPanel();
	}

	OpenPanel();
}

void UWidgetStorage::ClosePanel()
{
	Super::ClosePanel();
	m_InvenPanel->ClosePanel();
	m_StoragePanel->ClosePanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Bag->ClosePanel();
	}
	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_Belt->ClosePanel();
	}
}

void UWidgetStorage::OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	ele->SetTextFocus(NSLOCTEXT("UWidgetStorage","Deposite","넣기?"));
}

void UWidgetStorage::OnStorageInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	ele->SetTextFocus(NSLOCTEXT("UWidgetStorage","WithDraw","꺼내기?"));
}

void UWidgetStorage::OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	FItemSpec Item = inven->GetItemConstRef(index);
	inven->ClearSlot(index);
	inven->RemoveItemKey(Item.m_ID,index);
	m_StoragePanel->GetInven()->AddItem(Item);
	inven->UpdateInventory();
}

void UWidgetStorage::OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	FItemSpec Item = inven->GetItemConstRef(index);
	inven->ClearSlot(index);
	inven->RemoveItemKey(Item.m_ID,index);
	UMyGameInstance::Get->m_EquipManager->AddItem(Item);
	inven->UpdateInventory();
}
