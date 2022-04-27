#include "WidgetStorage.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_InvenPanel->Init(UMyLib::GetPlayerInven(),EPanelType::StorageDeposit);
	
	m_FilterBtns->RegisterFilter(m_StoragePanel);
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);
}

void UWidgetStorage::SetTargetInven(UInventory* storage)
{
	m_TargetInven = storage;
	
	m_StoragePanel->Init(m_TargetInven,EPanelType::StorageWithdraw);
	
	m_StoragePanel->OpenPanel();
	m_InvenPanel->OpenPanel();

	UpdateText();
	OpenPanel();

	m_Handle = m_TargetInven->m_OnInvenChanged.AddUObject(this,&UWidgetStorage::UpdateText);
}

void UWidgetStorage::ClosePanel()
{
	Super::ClosePanel();
	m_StoragePanel->ClosePanel();
	m_InvenPanel->ClosePanel();

	m_TargetInven->m_OnInvenChanged.Remove(m_Handle);
	m_TargetInven = nullptr;
}

void UWidgetStorage::UpdateText()
{
	UInventory* CrntStorage = m_StoragePanel->GetInven();
	
	int CurrentCount = CrntStorage->GetUsingSlotCount();
	
	int MaxCount = CrntStorage->GetInvenSize();

	FText StorageText = NSLOCTEXT("UWidgetStorage","StorageText","창고");
	
	FString StorageStr = FString::Printf(TEXT("%s %d/%d"),*StorageText.ToString(),CurrentCount,MaxCount);
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}

UInventory* UWidgetStorage::GetTargetInven()
{
	return m_TargetInven;
}
