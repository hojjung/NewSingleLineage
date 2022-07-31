#include "WidgetStorage.h"
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

	m_Bag->m_OnFocus.AddUObject(this, &UWidgetStorage::OnPlInvenFocused);

	m_Bag->m_OnFocusConfirm.AddUObject(this, &UWidgetStorage::OnPlInvenFocuseConfirm);

	m_BtnDepositAll->OnClicked.AddDynamic(this, &UWidgetStorage::OnDepositAll);
	m_BtnWithdrawAll->OnClicked.AddDynamic(this, &UWidgetStorage::OnWithdrawAll);
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

	OpenPanel();

	UpdateText();
}

void UWidgetStorage::OpenPanel()
{
	Super::OpenPanel();

	UMyLib::GetPlayer()->SetInteracting(true);
}

void UWidgetStorage::ClosePanel()
{
	Super::ClosePanel();
	
	UMyLib::GetPlayer()->SetInteracting(false);
	
	m_InvenPanel->ClosePanel();
	m_StoragePanel->ClosePanel();

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Bag->ClosePanel();
	}
}

void UWidgetStorage::AddRemoveItem(UInventory* from, FAddItem to, int index)
{
	FItemSpec& ItemRef = from->GetItemRef(index);

	if(ItemRef.m_ID.IsNone())
	{
		return;
	}
		
	int BeforeStack = ItemRef.m_nLvStack;
		
	if(to.Execute(ItemRef,false))
	{
		from->RemoveItemKey(ItemRef.m_ID,index);
		from->ClearSlot(index);
	}
	else
	{
		from->RemoveItem(ItemRef.m_ID,BeforeStack - ItemRef.m_nLvStack);
	}
}

void UWidgetStorage::AddRemoveItemAll(UInventory* from, FAddItem to)
{
	int MaxStorage = from->GetInvenSize();

	int Iter = -1;
	while (++Iter < MaxStorage)
	{
		AddRemoveItem(from, to, Iter);
	}
	from->UpdateInventory();
}

void UWidgetStorage::OnWithdrawAll()//모두꺼내기
{
	AddRemoveItemAll(m_StoragePanel->GetInven(),FAddItem::CreateUObject(UMyGameInstance::Get->m_EquipManager, &UEquipManager::AddItem));
}

void UWidgetStorage::OnDepositAll()
{
	AddRemoveItemAll(UMyGameInstance::Get->m_Inven,FAddItem::CreateUObject(m_StoragePanel->GetInven(), &UInventory::AddItem));

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		AddRemoveItemAll(UMyGameInstance::Get->m_EquipManager->GetBag(),FAddItem::CreateUObject(m_StoragePanel->GetInven(), &UInventory::AddItem));
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
	AddRemoveItem(inven, FAddItem::CreateUObject(m_StoragePanel->GetInven(), &UInventory::AddItem), index);
	inven->UpdateInventory();
}

void UWidgetStorage::OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index)
{
	AddRemoveItem(inven, FAddItem::CreateUObject(UMyGameInstance::Get->m_EquipManager, &UEquipManager::AddItem), index);
	inven->UpdateInventory();
}

void UWidgetStorage::UpdateText()
{
	m_TextStorageName->SetText(m_StoragePanel->GetInven()->GetInvenName());
}
