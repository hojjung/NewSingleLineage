#include "WidgetCraftSelected.h"
#include "WidgetCraftCostElement.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftSelected::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Icon->SetDragable(false);

	m_Icon->SetFocusable(false);

	m_Icon->SetHoldable(false);
}

void UWidgetCraftSelected::SelectCraft(const FCraftable& data)
{
	check(m_ClassCost);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Dele = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);

	m_Icon->SetIcon(((FEntityRow*)data.m_Row)->m_Icon);

	m_Wrap->ClearChildren();
	
	const TArray<FCraftItemCost>& Ary = data.GetAryCraftCosts();
	
	for(const FCraftItemCost& CraftData : Ary)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCost);
		
		SelectButton->SetCraftCost(CraftData);
		
		SelectButton->SetPadding(FMargin(0,30,0,0));
		
		m_Wrap->AddChild(SelectButton);
	}
}

void UWidgetCraftSelected::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_Dele);
}

void UWidgetCraftSelected::UpdateCraftCostPanel()
{
	if(!UMyGameInstance::Get->m_CraftManager->GetCrntItemRow())
	{
		return;
	}
	for(UWidgetCraftCostElement* Ele : m_AryEle)
	{
		Ele->UpdateCostAmount();
	}
}