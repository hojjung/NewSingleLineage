#include "WidgetHunger.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetHunger::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BarHunger->SetVisibility(ESlateVisibility::HitTestInvisible);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnHungerChanged.AddUObject(this, &UWidgetHunger::UpdateHunger);
	
	ShowNumber(m_bShowNumber);

	UpdateHunger(UMyGameInstance::Get->m_PlayerStatManager->GetHungerHP());
}

void UWidgetHunger::UpdateHunger(float hungerHp)
{
	m_TextHunger->SetText(FText::AsNumber((int)hungerHp));

	float Percent = 0;
	if(hungerHp > 0.f)
	{
		Percent = hungerHp / 100.f;
	}
	m_BarHunger->SetPercent(Percent);

	if(hungerHp <= 1.f)
	{
		SetColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		SetColorAndOpacity(FLinearColor::White);
	}
}

void UWidgetHunger::ShowNumber(bool b)
{
	if(b)
	{
		m_TextHunger->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_TextHunger->SetVisibility(ESlateVisibility::Collapsed);
	}
}
