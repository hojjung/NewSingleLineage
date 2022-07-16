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

void UWidgetPickpocketPanel::OnWithdrawAll()
{
	Super::OnWithdrawAll();

	if(m_StoragePanel->GetInven()->IsEmpty())
	{
		ClosePanel();
		
		if(!GetCurrentTargetPawn()->IsInteractable())
		{
			GetCurrentTargetPawn()->HideIcon();
		}
	}
}
