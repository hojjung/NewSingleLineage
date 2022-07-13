#include "WidgetLevelHealthInfo.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UWidgetLevelHealthInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_PlayerStatManager->m_OnExpChanged.AddUObject(this,&UWidgetLevelHealthInfo::UpdateLevel);
	UMyGameInstance::Get->m_PlayerStatManager->m_OnPlayerHpChanged.AddUObject(this, &UWidgetLevelHealthInfo::UpdateHp);
	
	UpdateHp(UMyLib::GetPlayer()->GetStat());
	UpdateLevel();
}

void UWidgetLevelHealthInfo::UpdateLevel()
{
	FString Str = FString::Printf(TEXT("Lv.%d"),UMyGameInstance::Get->m_PlayerStatManager->GetLevel());

	m_TextLevel->SetText(FText::FromString(Str));
}

void UWidgetLevelHealthInfo::UpdateHp(const FStatGroup& stat)
{
	float Per = 0.f;
	
	if(stat.m_Hp > 0.f)
	{
		Per = stat.m_Hp / stat.m_MaxHp;
	}
	
	m_HpBar->SetPercent(Per);
	//
	FString Str = FString::Printf(TEXT("%d/%d"), (int)stat.m_Hp, (int)stat.m_MaxHp);
	
	m_TextAmount->SetText(FText::FromString(Str));
}
