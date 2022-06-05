#include "ExpGoldWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UExpGoldWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnExpEarned.AddUObject(this,&UExpGoldWidget::ShowExp);
}

void UExpGoldWidget::ShowExp(float exp)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FString ExpStr = FString::Printf(TEXT("Exp %.0f"),exp);

	m_TextExp->SetText(FText::FromString(ExpStr));

	PlayAnimation(ShowHide);
}

void UExpGoldWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	SetVisibility(ESlateVisibility::Collapsed);
}
