#include "WidgetLevelHealthInfo.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UWidgetLevelHealthInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_PlayerStatManager->m_OnExpChanged.AddUObject(this,&UWidgetLevelHealthInfo::UpdateLevel);
	UMyGameInstance::Get->m_PlayerStatManager->m_OnPlayerHpChanged.AddUObject(this, &UWidgetLevelHealthInfo::UpdateHp);
	
	UpdateHp(UMyLib::GetPlayer());
	UpdateLevel();
}

void UWidgetLevelHealthInfo::UpdateLevel()
{
	FString Str = FString::Printf(TEXT("Lv.%d"),UMyGameInstance::Get->m_PlayerStatManager->GetLevel());

	m_TextLevel->SetText(FText::FromString(Str));
}

void UWidgetLevelHealthInfo::UpdateHp(const AMyPlayerPawn* pl)
{
	m_HpBar->SetPercent(pl->GetHpPercent());
}
