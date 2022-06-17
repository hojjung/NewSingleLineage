#include "WidgetPickpocketPanel.h"
#include "Components/WrapBoxSlot.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPickpocketPanel::SetTargetPawn(AMonsterPawn* targetPawn)
{
	m_TargetPawn = targetPawn;

	SetTargetInven(m_TargetPawn->GetInven());
	
	UpdateText();
}

AMonsterPawn* UWidgetPickpocketPanel::GetCurrentTargetPawn()
{
	return m_TargetPawn.Get();
}