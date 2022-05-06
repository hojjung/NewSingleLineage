#include "WidgetStorage.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_InvenPanel->Init(UMyLib::GetPlayerInven());

	m_InvenPanel->m_OnFocus.AddUObject(this, &UWidgetStorage::OnPlInvenFocused);

	m_InvenPanel->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnPlInvenFocuseConfirm);

	m_StoragePanel->m_OnFocus.AddUObject(this, &UWidgetStorage::OnStorageInvenFocused);

	m_StoragePanel->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnStorageFocuseConfirm);
}

void UWidgetStorage::SetTargetInven(UInventory* storage)
{
	m_StoragePanel->Init(storage);
	
	m_StoragePanel->OpenPanel();
	
	m_InvenPanel->OpenPanel();

	OpenPanel();
}

void UWidgetStorage::ClosePanel()
{
	Super::ClosePanel();
	m_InvenPanel->ClosePanel();
	m_StoragePanel->ClosePanel();
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
	inven->RemoveItem(index);
	m_StoragePanel->GetInven()->AddItem(Item);
	inven->UpdateInventory();
}

void UWidgetStorage::OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	FItemSpec Item = inven->GetItemConstRef(index);
	inven->RemoveItem(index);
	m_InvenPanel->GetInven()->AddItem(Item);
	inven->UpdateInventory();
}
