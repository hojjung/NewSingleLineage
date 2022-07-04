#include "WidgetHunger.h"

#include "MyJrpg/WidgetAnimLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetHunger::NativePreConstruct()
{
	Super::NativePreConstruct();

	ShowNumber(m_bShowNumber);
}

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
	float OldPercent = m_BarHunger->Percent;
	
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

	if(m_TextHunger->IsVisible())
	{
		m_TextHunger->SetText(FText::AsNumber((int)hungerHp));

		FLinearColor ColorW;
		
		if(OldPercent < Percent)
		{
			ColorW =  FLinearColor::Green;
		}
		else
		{
			ColorW =  FLinearColor::Red;
		}
		UWidgetAnimLib::PlayTextAnim(m_TextHunger, ColorW,0.5f);
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
