#include "WidgetStatPanel.h"
#include "TextText.h"
#include "Components/ScrollBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetStatPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CreateText(9);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnStatChanged.AddUObject(this, &UWidgetStatPanel::UpdateStats);
	
	UpdateStats();
}

void UWidgetStatPanel::CreateText(int max)
{
	m_AryText.Reset(max);
	
	int	iter = 0;

	while (iter < max)
	{
		UWidgetTextText* WidgetT = NewObject<UWidgetTextText>(this);
		FSlateFontInfo Font = WidgetT->LeftFont;

		Font.OutlineSettings.OutlineSize = 2;

		WidgetT->LeftFont = Font;
		WidgetT->RightFont = Font;

		UScrollBoxSlot* SlotChild = Cast<UScrollBoxSlot>(m_ScrollBarStat->AddChild(WidgetT));

		SlotChild->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		SlotChild->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		
		m_AryText.Add(WidgetT);
		iter++;
	}
}

void UWidgetStatPanel::UpdateStats()
{
	const FStatGroup& Stat = UMyGameInstance::Get->m_PlayerStatManager->GetStat();

	AddStatText(NSLOCTEXT("UWidgetStatPanel","MaxHp","MaxHp"), UMyLib::SetFloatPrecision(Stat.m_MaxHp,0),0);
	
	AddStatText(NSLOCTEXT("UWidgetStatPanel","Damage","Damage"), UMyLib::SetFloatPrecision(Stat.m_Dmg,0),1);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","AtkSpeed","AtkSpeed"), UMyLib::SetFloatPrecision(Stat.m_AtkPerSec),2);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","Cri-Per","Cri-Per"), UMyLib::GetFloatToPercentText(Stat.m_CriPer),3);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","Cri-Dmg","Cri-Dmg"), UMyLib::GetFloatToPercentText(Stat.m_CriDmg),4);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","Accuracy","Accuracy"), Stat.m_nAccu,5);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","Avoid","Avoid"), Stat.m_nAvoid,6);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","Dmg-Reduc","Dmg-Reduc"), Stat.m_DmgReduce,7);

	AddStatText(NSLOCTEXT("UWidgetStatPanel","MoveSpeed","MoveSpeed"), UMyLib::GetFloatToPercentText(Stat.m_MoveSpeed / FGlobalVariable::HERO_DEFAULT_SPEED),8);
}

void UWidgetStatPanel::AddStatText(FText statName, float v, int index)
{
	AddStatText(statName,FText::AsNumber(v),index);
}

void UWidgetStatPanel::AddStatText(FText statName, FText statValue, int index)
{
	UWidgetTextText* WidgetT = m_AryText[index];
	
	m_ScrollBarStat->AddChild(WidgetT);

	WidgetT->SetLeftText(statName);

	WidgetT->SetRightText(statValue);
}
