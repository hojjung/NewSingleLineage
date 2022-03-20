#include "ExpGoldWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UExpGoldWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_RewardManager->m_OnExpGold.AddUObject(this,&UExpGoldWidget::ShowExpGold);
}

void UExpGoldWidget::ShowExpGold(float exp, float gold)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FString ExpStr = FString::Printf(TEXT("Exp %.0f"),exp);

	FString GoldStr = FString::Printf(TEXT("%.0f"),gold);

	m_TextExp->SetText(FText::FromString(ExpStr));

	m_ImgTextGold->SetText(FText::FromString(GoldStr));

	PlayAnimation(ShowHide);
}

void UExpGoldWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	SetVisibility(ESlateVisibility::Collapsed);
}
