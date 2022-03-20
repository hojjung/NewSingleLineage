#include "WidgetPawnInfo.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UWidgetPawnInfo::SetPawnInfo(const ACombatUnitPawn* pawn)
{
	FText NamePawn =  pawn->GetPawnName();
	
	m_TextPawnName->SetText(NamePawn);

	float HpPercent = pawn->GetHpPercent();

	m_HpBar->SetPercent(HpPercent);
}