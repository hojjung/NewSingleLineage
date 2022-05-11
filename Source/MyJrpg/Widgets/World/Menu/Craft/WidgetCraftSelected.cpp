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

	m_BtnCancel->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCancel->OnClicked.AddDynamic(this, &UWidgetCraftSelected::Cancel);
	
	m_OverlayLimit->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetCraftSelected::SelectCraft(const FCraftDataInfo& data)
{
	check(m_ClassCost);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Dele = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);

	m_Icon->SetIcon(data.m_ItemData->m_Icon);

	m_Wrap->ClearChildren();
	
	const TArray<FCraftItemCost>& Ary = data.m_ItemData->m_AryCostItem;
	
	for(const FCraftItemCost& CraftData : Ary)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCost);
		
		SelectButton->SetCraftCost(CraftData);
		
		m_Wrap->AddChild(SelectButton);
	}

	m_TextName->SetText(data.m_ItemData->m_ShowingName);
	m_TextDesc->SetText(data.m_ItemData->m_Desc);

	//SetLimitLevel(data);
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

void UWidgetCraftSelected::Cancel()
{
	
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

void UWidgetCraftSelected::SetLimitLevel(const FCraftDataInfo& data)
{
	int Level = data.m_ItemData->m_nCraftLevelLimit;

	FTextFormat FormatT = FTextFormat::FromString(TEXT("{0} {1} {2}"));

	FFormatOrderedArguments Args;

	Args.Add(NSLOCTEXT("UWidgetCraftSelected","Level","레벨"));

	Args.Add(Level);

	Args.Add(NSLOCTEXT("UWidgetCraftSelected","Needs","필요"));

	m_TextLevelLimit->SetText(FText::Format(FormatT,Args));

	bool IsLevelAble = UMyGameInstance::Get->m_PlayerStatManager->GetLevel() >= Level;

	if(IsLevelAble)
	{
		m_OverlayLimit->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnCraft->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_OverlayLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_BtnCraft->SetVisibility(ESlateVisibility::Collapsed);
	}
}
