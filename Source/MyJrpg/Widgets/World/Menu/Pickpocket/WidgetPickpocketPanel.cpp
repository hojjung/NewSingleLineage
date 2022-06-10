#include "WidgetPickpocketPanel.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPickpocketPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

AMonsterPawn* UWidgetPickpocketPanel::GetCurrentTargetPawn()
{
	return m_TargetPawn;
}

void UWidgetPickpocketPanel::UpdateText()
{
	FText PocketText = NSLOCTEXT("UWidgetPickpocketPanel","Pocket","의 주머니");
	
	FText OwnerNameText = m_TargetPawn->GetPawnName();
	
	FString StorageStr = FString::Printf(TEXT("%s%s"),*OwnerNameText.ToString(),*PocketText.ToString());
	//
	m_TxtStorageInvenCount->SetText(FText::FromString(StorageStr));
}

void UWidgetPickpocketPanel::SetTargetPawn(AMonsterPawn* targetPawn)
{
	m_TargetPawn = targetPawn;

	SetTargetInven(m_TargetPawn->GetInven());
	
	UpdateText();
}