#include "WidgetCollecStatParent.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCollecStatParent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Manager = UMyGameInstance::Get->m_ItemCollecManager;
	
	CreateStats();

	UpdateStats();

	UpdateTotalProgress();
}

void UWidgetCollecStatParent::CreateStats()
{
	m_Scroll->ClearChildren();
	
	for(auto Collec : m_Manager->GetMapOptionGroups())
	{
		UWidgetCollecStatChild* WidgetChild = CreateWidget<UWidgetCollecStatChild>(this, m_ClassStatChild);

		WidgetChild->SetStatText(Collec);
		
		m_Scroll->AddChild(WidgetChild);
	}
}

void UWidgetCollecStatParent::UpdateTotalProgress()
{
	m_TotalPercent->SetText(m_Manager->GetTotalProgressText());

	float Per = m_Manager->GetTotalProgress();

	m_ProgressBar->SetPercent(Per);

	FString Str = FString::Printf(TEXT("%2.1f"),Per);
	
	m_TotalPercent->SetText(FText::FromString(Str));
}

void UWidgetCollecStatParent::UpdateStats()
{
	int Iter = 0;
	
	for(auto Collec : m_Manager->GetMapOptionGroups())
	{
		UWidgetCollecStatChild* StatChild = Cast<UWidgetCollecStatChild>(m_Scroll->GetChildAt(Iter));
		
		StatChild->SetStatText(Collec);
		
		Iter++;
	}
}
