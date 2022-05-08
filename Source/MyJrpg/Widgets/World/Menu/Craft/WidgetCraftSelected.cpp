#include "WidgetCraftSelected.h"
#include "WidgetCraftCostElement.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetCraftSelected::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetVisibility(ESlateVisibility::Collapsed);

	m_Icon->SetDragable(false);

	m_Icon->SetFocusable(false);

	m_Icon->SetHoldable(false);

	m_BtnCraft->OnClicked.AddDynamic(this, &UWidgetCraftSelected::Craft);
}

void UWidgetCraftSelected::SelectCraft(const FCraftable& data)
{
	check(m_ClassCost);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Dele = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);

	m_Icon->SetIcon(data.GetEntityRow()->m_Icon);

	m_Wrap->ClearChildren();
	
	const TArray<FCraftItemCost>& Ary = data.GetAryCraftCosts();
	
	for(const FCraftItemCost& CraftData : Ary)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCost);
		
		SelectButton->SetCraftCost(CraftData);
		
		m_Wrap->AddChild(SelectButton);
	}

	m_TextName->SetText(data.GetEntityRow()->m_ShowingName);
	m_TextDesc->SetText(data.GetEntityRow()->m_Desc);
}

void UWidgetCraftSelected::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_Dele);
}

void UWidgetCraftSelected::Craft()
{
	UMyGameInstance::Get->m_CraftManager->TryCraft();
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
