#include "WidgetPickpocketPanel.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPickpocketPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	m_nCrntIndex = 0;

	m_InvenPanel->Init(UMyLib::GetPlayerInven(),EPanelType::StorageDeposit);
	
	m_FilterBtns->RegisterFilter(m_OtherPanel);
	
	m_FilterBtns->RegisterFilter(m_InvenPanel);
}

void UWidgetPickpocketPanel::OpenPanel()
{
	Super::OpenPanel();
	AddInvenDelegate();
	m_InvenPanel->OpenPanel();
}

void UWidgetPickpocketPanel::ClosePanel()
{
	Super::ClosePanel();

	RemoveInvenDelegate();
	m_InvenPanel->ClosePanel();

	m_TargetPawn = nullptr;
}

void UWidgetPickpocketPanel::UpdateText()
{
	FText PocketText = NSLOCTEXT("UWidgetPickpocketPanel","Pocket","의 주머니");
	
	FText OwnerNameText = m_TargetPawn->GetPawnName();
	
	FString StorageStr = FString::Printf(TEXT("%s%s"),*OwnerNameText.ToString(),*PocketText.ToString());
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}

void UWidgetPickpocketPanel::AddInvenDelegate()
{
	m_EachInvenHandle = m_TargetPawn->GetInven()->m_OnInvenChanged.AddUObject(this,&UWidgetPickpocketPanel::UpdateText);

	m_OtherPanel->Init(m_TargetPawn->GetInven(),EPanelType::StorageWithdraw);
	m_OtherPanel->OpenPanel();
	
	UpdateText();
}

void UWidgetPickpocketPanel::RemoveInvenDelegate()
{
	m_OtherPanel->ClosePanel();
	m_TargetPawn->GetInven()->m_OnInvenChanged.Remove(m_EachInvenHandle);
}

void UWidgetPickpocketPanel::SetTargetPawn(AMonsterPawn* targetPawn)
{
	m_TargetPawn = targetPawn;
}