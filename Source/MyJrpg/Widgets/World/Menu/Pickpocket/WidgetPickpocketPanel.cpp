#include "WidgetPickpocketPanel.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPickpocketPanel::OpenPanel()
{
	Super::OpenPanel();

	UMyLib::GetPlayer()->SetInteracting(true);
}

void UWidgetPickpocketPanel::ClosePanel()
{
	Super::ClosePanel();
	
	UMyLib::GetPlayer()->SetInteracting(false);
}

void UWidgetPickpocketPanel::SetTargetPawn(AMonsterPawn* targetPawn)
{
	m_TargetPawn = targetPawn;

	SetTargetInven(m_TargetPawn->GetInven());
	
	UpdateText();
}

AMonsterPawn* UWidgetPickpocketPanel::GetCurrentTargetPawn()
{
	return m_TargetPawn.Get();
}

void UWidgetPickpocketPanel::UpdateText()
{
	FText PocketText = NSLOCTEXT("UWidgetPickpocketPanel","Pocket","의 주머니");
	
	FText OwnerNameText = m_TargetPawn->GetPawnName();
	
	FString StorageStr = FString::Printf(TEXT("%s%s"),*OwnerNameText.ToString(),*PocketText.ToString());
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}