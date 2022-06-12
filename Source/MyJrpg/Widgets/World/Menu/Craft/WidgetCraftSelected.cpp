#include "WidgetCraftSelected.h"
#include "WidgetCraftCostElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
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

	m_fCancelTimer = 0;

	m_bStartCraft = false;
}

void UWidgetCraftSelected::SelectCraft(const FCraftDataInfo& data)
{
	check(m_ClassCost);
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_Dele = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_Dele2 = UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);
	}
	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_Dele3 = UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().AddUObject(this, &UWidgetCraftSelected::UpdateCraftCostPanel);
	}

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

	if(!UMyLib::IsTestMode())
	{
		SetLimitLevel(data);
	}
	UpdateCraftCostPanel();
}

void UWidgetCraftSelected::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_Dele);

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().Remove(m_Dele2);
	}
	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().Remove(m_Dele3);
	}

	Cancel();
}

void UWidgetCraftSelected::Craft()
{
	if(!UMyGameInstance::Get->m_CraftManager->CheckCraftable())
	{
		return;
	}
	m_BtnCancel->SetVisibility(ESlateVisibility::Visible);
	
	m_bStartCraft = true;

	m_fCancelTimer = 0;
}

void UWidgetCraftSelected::Cancel()
{
	m_bStartCraft = false;

	m_fCancelTimer = 0;

	m_BtnCancel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UWidgetCraftSelected::IsCrafting()
{
	return m_bStartCraft;
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
	
	bool IsLevelAble = UMyGameInstance::Get->m_PlayerStatManager->GetLevel() >= Level;

	bool IsCountEnough = UMyGameInstance::Get->m_BuildManager->HasFurnitureEmptySpace(data.m_ID);

	if(IsLevelAble && IsCountEnough)
	{
		m_OverlayLimit->SetVisibility(ESlateVisibility::Collapsed);
		
		m_BtnCraft->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_BtnCraft->SetVisibility(ESlateVisibility::Collapsed);
		
		m_OverlayLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if(!IsLevelAble)
		{
			m_TextCountLimit->SetVisibility(ESlateVisibility::Collapsed);
			
			m_TextLevelLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			
			FTextFormat FormatT = FTextFormat::FromString(TEXT("{0} {1} {2}"));
			FFormatOrderedArguments Args;
			Args.Add(NSLOCTEXT("UWidgetCraftSelected","Level","레벨"));
			Args.Add(Level);
			Args.Add(NSLOCTEXT("UWidgetCraftSelected","Needs","필요"));
			m_TextLevelLimit->SetText(FText::Format(FormatT,Args));
		}
		else if (!IsCountEnough)
		{
			m_TextLevelLimit->SetVisibility(ESlateVisibility::Collapsed);
			
			m_TextCountLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			int MaxCnt = UMyGameInstance::Get->m_BuildManager->GetFurnitureMaxOwnCnt(data.m_ID);
			FTextFormat FormatT = FTextFormat::FromString(TEXT("{0}{1}"));
			FFormatOrderedArguments Args;
			Args.Add(NSLOCTEXT("UWidgetCraftSelected","MaxOwnedCnt","최대 보유 개수:"));
			Args.Add(MaxCnt);
			m_TextCountLimit->SetText(FText::Format(FormatT, Args));
		}
	}
}

void UWidgetCraftSelected::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!m_bStartCraft)
	{
		return;
	}
	m_fCancelTimer += InDeltaTime;

	m_CancelHoldingBar->SetPercent(m_fCancelTimer / 1.f);

	if(m_fCancelTimer > 1.f)
	{
		Cancel();
		UMyGameInstance::Get->m_CraftManager->Craft();
	}
}
