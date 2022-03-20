#include "AlertInfoElement.h"

void UAlertInfoElement::Show(const FString& str)
{
	StopAllAnimations();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_TextName->SetText(FText::FromString(str));
	PlayAnimation(ShowHide);
}

void UAlertInfoElement::Hide()
{
	StopAllAnimations();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAlertInfoElement::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	Hide();
}

