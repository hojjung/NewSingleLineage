#include "WidgetRideCost.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetRideCost::TryVisible()
{
	if(UMyGameInstance::Get->m_BuildManager->HasFurniturePlaced(TEXT("Horse")))
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		UMyGameInstance::Get->m_ZoneMove->m_OnRideCostChanged.AddUObject(this, &UWidgetRideCost::UpdateCostText);
	
		UpdateCostText();	
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetRideCost::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_CostBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	TryVisible();	
}

void UWidgetRideCost::UpdateCostText()
{
	int Stamina = UMyGameInstance::Get->m_ZoneMove->GetRideEnergy();
	
	float Per = 0.f;

	if(Stamina > 0)
	{
		Per = (float)Stamina / 100.f;
	}
	
	m_CostBar->SetPercent(Per);
	
	FString Str = FString::Printf(TEXT("%d/100"),Stamina);
	
	m_TextAmount->SetText(FText::FromString(Str));	
}