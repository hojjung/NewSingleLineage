#include "WidgetStorage.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nCrntIndex = 0;

	m_InvenPanel->Init(UMyLib::GetPlayerInven(),EPanelType::StorageDeposit);
	
	m_FilterBtns->RegisterFilter(m_StoragePanel);
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);

	m_BtnLeft->OnClicked.AddDynamic(this, &UWidgetStorage::OnClickLeft);
	
	m_BtnRight->OnClicked.AddDynamic(this, &UWidgetStorage::OnClickRight);
}

void UWidgetStorage::OpenPanel()
{
	Super::OpenPanel();
	AddInvenDelegate();
	m_InvenPanel->OpenPanel();
}

void UWidgetStorage::ClosePanel()
{
	Super::ClosePanel();

	RemoveInvenDelegate();
	m_InvenPanel->ClosePanel();
}

void UWidgetStorage::UpdateText()
{
	UInventory* CrntStorage = UMyLib::GetPlayerStorage();
	
	int CurrentCount = CrntStorage->GetUsingSlotCount();
	
	int MaxCount = CrntStorage->GetInvenSize();

	int Index = UMyGameInstance::Get->GetCrntStorageIndex() + 1;
	
	FText StorageText = NSLOCTEXT("UWidgetStorage","StorageText","창고");
	
	FString StorageStr = FString::Printf(TEXT("%s(%d) %d/%d"),*StorageText.ToString(),Index,CurrentCount,MaxCount);
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}

void UWidgetStorage::AddInvenDelegate()
{
	UMyGameInstance::Get->SelectStorage(m_nCrntIndex);

	m_EachInvenHandle = UMyLib::GetPlayerStorage()->m_OnInvenChanged.AddUObject(this,&UWidgetStorage::UpdateText);

	m_StoragePanel->Init(UMyLib::GetPlayerStorage(),EPanelType::StorageWithdraw);
	m_StoragePanel->OpenPanel();
	
	UpdateText();
}

void UWidgetStorage::RemoveInvenDelegate()
{
	m_StoragePanel->ClosePanel();
	UMyLib::GetPlayerStorage()->m_OnInvenChanged.Remove(m_EachInvenHandle);
}



void UWidgetStorage::OnClickLeft()
{
	RemoveInvenDelegate();
	
	m_nCrntIndex--;

	m_nCrntIndex = FMath::Max(m_nCrntIndex,0);

	AddInvenDelegate();
}

void UWidgetStorage::OnClickRight()
{
	RemoveInvenDelegate();
	
	m_nCrntIndex++;
	
	m_nCrntIndex = FMath::Min(m_nCrntIndex,3);

	AddInvenDelegate();
}
