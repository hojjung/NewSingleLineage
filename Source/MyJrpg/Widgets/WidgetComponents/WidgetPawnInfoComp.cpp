// All right Reserve 2021 HereticByte


#include "WidgetPawnInfoComp.h"
#include "WidgetPawnInfo.h"

UWidgetPawnInfoComp::UWidgetPawnInfoComp()
{
	SetManuallyRedraw(true);
}

void UWidgetPawnInfoComp::SetPawnInfo(const ACombatUnitPawn* pawn)
{
	UUserWidget* WidgetObj = GetUserWidgetObject();

	if (!WidgetObj)
	{
		return;
	}
	
	UWidgetPawnInfo* WidgetProgressBar = Cast<UWidgetPawnInfo>(WidgetObj);

	WidgetProgressBar->SetPawnInfo(pawn);

	RequestRedraw();
}
