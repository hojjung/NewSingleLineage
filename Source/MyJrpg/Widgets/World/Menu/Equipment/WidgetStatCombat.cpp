// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStatCombat.h"

#include "MyJrpg/WidgetAnimLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStatCombat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_OldDmg = 0;
	
	m_OldDef = 0;
	
	m_OldMSpeed = 0;

	m_OldAtkSpeed = 0;

	UMyGameInstance::Get->m_EquipManager->m_OnEquipChanged.AddUObject(this, &UWidgetStatCombat::UpdateText);

	UpdateText();
}

void UWidgetStatCombat::UpdateText()
{
	float NewDmg = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_Dmg;

	float NewDef = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_DmgReduce;

	float NewMSpeed = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_MoveSpeed;

	float NewAtkSpeed = UMyGameInstance::Get->m_PlayerStatManager->GetStat().m_AtkPerSec;

	SetTextColorAnim(NewDmg, m_OldDmg, m_TextDmg);

	SetTextColorAnim(NewDef, m_OldDef, m_TextDef);

	SetTextColorAnim(NewMSpeed, m_OldMSpeed, m_TextMSpeed);

	SetTextColorAnim(NewAtkSpeed, m_OldAtkSpeed, m_TextAtkSpeed);
}

void UWidgetStatCombat::SetTextColorAnim(float& newStat, float& oldStat, UTextBlock* textWant)
{
	FLinearColor ColorWant;
	
	if(oldStat < newStat)
	{
		ColorWant = FLinearColor::Green;
	}
	else if(oldStat == newStat)
	{
		oldStat = newStat;

		textWant->SetText(FText::AsNumber((int)newStat));
		
		return;
	}
	else
	{
		ColorWant = FLinearColor::Red;
	}
	UWidgetAnimLib::PlayTextAnim(textWant, ColorWant, 0.3f);

	textWant->SetText(FText::AsNumber((int)newStat));

	oldStat = newStat;
}
