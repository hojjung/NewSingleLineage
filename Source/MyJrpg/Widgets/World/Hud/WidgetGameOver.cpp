#include "WidgetGameOver.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetGameOver::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnReturnVillage->OnClicked.AddDynamic(this, &UWidgetGameOver::OnReturnVillage);
}

void UWidgetGameOver::OnReturnVillage()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(TEXT("Village"),0);
}

void UWidgetGameOver::SetKiller(const ACombatUnitPawn* killer)
{
	FString Str = FString::Printf(TEXT("%s의 공격으로 사망"),*killer->GetPawnName().ToString());

	m_TextDeadReason->SetText(FText::FromString(Str));
}
