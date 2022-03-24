#include "InGameTextWidget.h"

#include "InGameTextWidgetComp.h"

void UInGameTextWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryDmgTxtFuncs[static_cast<int>(ETextType::NormalRight)] = &UInGameTextWidget::PlayNormalDmg;

	m_AryDmgTxtFuncs[static_cast<int>(ETextType::CritcalRight)] = &UInGameTextWidget::PlayCriticalDmg;

	m_AryDmgTxtFuncs[static_cast<int>(ETextType::PlayerTookHeal)] = &UInGameTextWidget::PlayPlayerHeal;

	m_AryDmgTxtFuncs[static_cast<int>(ETextType::Miss)] = &UInGameTextWidget::PlayMiss;

	m_AryDmgTxtFuncs[static_cast<int>(ETextType::Immune)] = &UInGameTextWidget::PlayImmune;
}

void UInGameTextWidget::SetParentComponent(UInGameTextWidgetComp* parent)
{
	m_Parent = parent;
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameTextWidget::PlayNormalDmg()
{
	PlayAnimation(Dmg);
}

void UInGameTextWidget::PlayCriticalDmg()
{
	PlayAnimation(Critical);
}

void UInGameTextWidget::PlayPlayerHeal()
{
	PlayAnimation(Heal);
}

void UInGameTextWidget::PlayImmune()
{
	m_TextName->SetText(FText::FromString(TEXT("Immune")));
	
	PlayAnimation(Immune);
}

void UInGameTextWidget::PlayMiss()
{
	m_TextName->SetText(FText::FromString(TEXT("Miss")));
	
	PlayAnimation(Miss);
}

void UInGameTextWidget::SetTextWant(const FText& textWant, ETextType dmg)
{
	StopAllAnimations();
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextName->SetText(textWant);
	
	(this->*m_AryDmgTxtFuncs[static_cast<int>(dmg)])();

	m_Parent->RequestRedraw();
}

void UInGameTextWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	SetVisibility(ESlateVisibility::Collapsed);
	
	//m_Parent->SetRelativeLocation(FVector(0,0,0));
}
