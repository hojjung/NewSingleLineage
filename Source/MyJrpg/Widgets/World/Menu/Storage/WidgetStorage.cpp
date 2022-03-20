#include "WidgetStorage.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStorage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nCrntIndex = 0;

	UMyLib::GetPlayerInven()->m_OnInvenChanged.AddUObject(this,&UWidgetStorage::UpdateText);
	
	m_InvenPanel->Init(UMyLib::GetPlayerInven(),EPanelType::StorageDeposit);
	
	AddInvenDelegate();

	m_FilterBtns->RegisterFilter(m_StoragePanel);
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);

	m_BtnLeft->OnClicked.AddDynamic(this, &UWidgetStorage::OnClickLeft);
	
	m_BtnRight->OnClicked.AddDynamic(this, &UWidgetStorage::OnClickRight);

	UpdateText();
}

void UWidgetStorage::UpdateText()
{
	UInventory* CrntStorage = UMyLib::GetPlayerStorage();
	
	int CurrentCount = CrntStorage->GetEmptyIndex();

	int MaxCount = CrntStorage->m_nInvenMaxSize;

	FText StorageText = NSLOCTEXT("UWidgetStorage","StorageText","Storage");

	FString StorageStr = FString::Printf(TEXT("%s %d/%d"),*StorageText.ToString(),CurrentCount,MaxCount);
	//
	CurrentCount = UMyLib::GetPlayerInven()->GetEmptyIndex();

	MaxCount = UMyLib::GetPlayerInven()->m_nInvenMaxSize;

	FText InvenText = NSLOCTEXT("UWidgetStorage","InvenText","Inventory");

	FString InvenStr = FString::Printf(TEXT("%s %d/%d"),*InvenText.ToString(),CurrentCount,MaxCount);
	//
	FString TotalStr = FString::Printf(TEXT("%s %s"),*StorageStr,*InvenStr);

	m_TxtStorageInvenCount->SetText(FText::FromString(TotalStr));
}

void UWidgetStorage::AddInvenDelegate()
{
	UMyGameInstance::Get->SelectStorage(m_nCrntIndex);

	m_DeleHandle = UMyLib::GetPlayerStorage()->m_OnInvenChanged.AddUObject(this,&UWidgetStorage::UpdateText);

	m_StoragePanel->Init(UMyLib::GetPlayerStorage(),EPanelType::StorageWithdraw);
}

void UWidgetStorage::RemoveInvenDelegate()
{
	UMyLib::GetPlayerStorage()->m_OnInvenChanged.Remove(m_DeleHandle);
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
