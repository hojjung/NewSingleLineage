#include "WidgetCraftPanel.h"
#include "WidgetCraftableElement.h"
#include "WidgetCraftCostElement.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"

UWidgetCraftPanel::UWidgetCraftPanel(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundT (TEXT("Texture2D'/Game/Sprites/UI/HarmoBox/coin_01.coin_01'"));

	m_CoinIcon = FoundT.Object;
}

void UWidgetCraftPanel::NativeOnInitialized()//Texture2D'/Game/Sprites/UI/HarmoBox/coin_01.coin_01'
{
	Super::NativeOnInitialized();
	
	m_CountBtns->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCraft->SetVisibility(ESlateVisibility::Collapsed);
	
	m_GoldIcon->SetVisibility(ESlateVisibility::Collapsed);

	m_GoldIcon->SetIcon(m_CoinIcon);

	m_GoldIcon->SetHoldable(false);

	m_GoldIcon->SetFocusable(false);
	//모든 제작품목 가져옴
	CreateAllCraftWidget(UMyGameInstance::Get->m_CraftManager->GetCraftItems());

	m_CurrentCraftItem->SetVisibility(ESlateVisibility::Collapsed);

	m_TextCurrentItemName->SetVisibility(ESlateVisibility::Collapsed);

	m_TextGoldCostAmount->SetVisibility(ESlateVisibility::Collapsed);

	m_CurrentCraftItem->SetHoldable(false);

	m_BtnCraft->IsFocusable = false;

	m_BtnCraft->OnClicked.AddDynamic(this,&UWidgetCraftPanel::Craft);

	m_BtnOpenCalculator->IsFocusable = false;

	m_BtnOpenCalculator->OnClicked.AddDynamic(this,&UWidgetCraftPanel::OpenCalculator);

	m_BtnOpenCalculator->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnLeftCnt->OnClicked.AddDynamic(this,&UWidgetCraftPanel::OnLeftCnt);

	m_BtnRightCnt->OnClicked.AddDynamic(this,&UWidgetCraftPanel::OnRightCnt);

	m_ItemFilter->RegisterFilter(this);

	m_EquipFilter->RegisterFilter(this);

	m_ColorFilter->RegisterFilter(this);

	ClearFilter();

	UpdateCraftablePanel();
}

void UWidgetCraftPanel::CreateAllCraftWidget(const TMap<FName, const FCraftItemCost*>& mapItems)
{
	for(auto& CraftData : mapItems)
	{
		UWidgetCraftableElement* SelectButton = CreateWidget<UWidgetCraftableElement>(this,m_ClassCraftableElement);

		SelectButton->SetCraftable(CraftData.Key);

		SelectButton->m_OnClicked.AddUObject(this,&UWidgetCraftPanel::OnSelectCraftItem);

		m_ScrollCraftables->AddChild(SelectButton);

		SelectButton->SetPadding(FMargin(0,30,0,0));
	}
}

void UWidgetCraftPanel::OnSelectCraftItem(UWidgetCraftableElement* selectedElement, FName id)
{
	if(m_SelectedElement.Get())
	{
		m_SelectedElement->SetUnfocus();
	}

	UMyGameInstance::Get->m_CraftManager->SetCraftItem(id);

	m_SelectedElement = selectedElement;

	m_SelectedElement->SetMyFocus();

	SetAmount(1);
	
	const FItemDataRow& TargetItem = *UMyGameInstance::Get->m_CraftManager->GetCrntItemRow();
	
	CreateCostWidgets(TargetItem);

	m_TextCurrentItemName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_CurrentCraftItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_BtnOpenCalculator->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextGoldCostAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_CountBtns->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_BtnCraft->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_GoldIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//
	//
	m_CurrentCraftItem->SetIcon(TargetItem.m_ItemIcon);

	m_CurrentCraftItem->SetGlowColor(TargetItem.m_ColorHandle);

	m_TextCurrentItemName->SetText(TargetItem.m_TextShowingName);

	m_TextCurrentItemName->SetColorAndOpacity(TargetItem.m_ColorHandle.GetRow<FColorDataRow>("")->m_Color);
}

void UWidgetCraftPanel::CreateCostWidgets(const FItemDataRow& costData)
{
	m_AryEle.Reset();
	
	m_ScrollCosts->ClearChildren();
	
	for(const FCraftItemCost& CraftData : costData.m_AryCostItem)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCostElement);

		SelectButton->SetCraftCost(CraftData);

		m_ScrollCosts->AddChild(SelectButton);
		
		SelectButton->SetPadding(FMargin(0,30,0,0));

		m_AryEle.Add(SelectButton);
	}
}

void UWidgetCraftPanel::UpdateCraftablePanel()
{
	if (m_FilterCategoryItem == EItemType::Equip)
	{
		m_EquipFilter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_EquipFilter->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	for(UWidget* ChildWidget :  m_ScrollCraftables->GetAllChildren())
	{
		UWidgetCraftableElement* CraftWidget = Cast<UWidgetCraftableElement>(ChildWidget);

		const FItemDataRow& FoundItem = CraftWidget->GetItemDataRow();

		if (IsFilterType(FoundItem))
		{
			CraftWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CraftWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UWidgetCraftPanel::UpdateCraftCostPanel()
{
	for(UWidgetCraftCostElement* Ele : m_AryEle)
	{
		Ele->UpdateCostAmount();
	}

	int CurrentGold = UMyGameInstance::Get->m_CurrencyManager->GetGold();

	int TotalCostGold = UMyGameInstance::Get->m_CraftManager->GetTotalCost();

	FString Str = FString::Printf(TEXT("%d/%d"),CurrentGold,TotalCostGold);

	bool IsEnough = CurrentGold >= TotalCostGold; 

	m_TextGoldCostAmount->SetText(FText::FromString(Str));

	if(IsEnough)
	{
		m_TextGoldCostAmount->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		m_TextGoldCostAmount->SetColorAndOpacity(FLinearColor::Red);
	}
}

void UWidgetCraftPanel::SetAmount(int amount)
{
	m_nCnt = amount;
	
	if(m_nCnt<=0)
	{
		m_nCnt=1;
	}
	UMyGameInstance::Get->m_CraftManager->SetCraftAmount(m_nCnt);

	m_TextAmount->SetText(FText::AsNumber(m_nCnt));

	UpdateCraftCostPanel();
}

int UWidgetCraftPanel::GetMaxAmount()
{
	return 10;
}

void UWidgetCraftPanel::Craft()
{
	UMyGameInstance::Get->m_CraftManager->TryCraft();

	UpdateCraftCostPanel();
}

void UWidgetCraftPanel::OpenCalculator()
{
	UWidgetStackCalculator* Calcu = UMyLib::GetCanvas()->OpenCalculator(m_nCnt);

	Calcu->m_OnNumberAccept.AddUObject(this,&UWidgetCraftPanel::SetAmount);

	Calcu->m_OnGetMax.BindUObject(this,&UWidgetCraftPanel::GetMaxAmount);
}

void UWidgetCraftPanel::OnLeftCnt()
{
	m_nCnt--;

	m_nCnt = FMath::Max(m_nCnt, 1);

	SetAmount(m_nCnt);
}

void UWidgetCraftPanel::OnRightCnt()
{
	m_nCnt++;
	
	m_nCnt = FMath::Min(m_nCnt, GetMaxAmount());

	SetAmount(m_nCnt);
}

void UWidgetCraftPanel::ClosePanel()
{
	Super::ClosePanel();
	
	if(m_SelectedElement.Get())
	{
		m_SelectedElement->SetUnfocus();

		m_SelectedElement = nullptr;
	}

	m_AryEle.Reset();
	
	m_ScrollCosts->ClearChildren();

	m_TextCurrentItemName->SetVisibility(ESlateVisibility::Collapsed);
	
	m_CurrentCraftItem->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnOpenCalculator->SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_CraftManager->Clear();

	m_TextGoldCostAmount->SetVisibility(ESlateVisibility::Collapsed);

	m_CountBtns->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCraft->SetVisibility(ESlateVisibility::Collapsed);

	m_GoldIcon->SetVisibility(ESlateVisibility::Collapsed);
}

bool UWidgetCraftPanel::IsFilterType(const FItemDataRow& itemData)
{
	bool bColorOkay = false;
	
	bool bItemTypeOkay = false;
	
	bool bEquipTypeOkay = false;
	
	if (m_FilterColorData == NAME_None)
	{
		bColorOkay = true;
	}
	else
	{
		bColorOkay = m_FilterColorData == itemData.m_ColorHandle.RowName;
	}

	if (m_FilterCategoryItem == EItemType::None)
	{
		bItemTypeOkay = true;
	}
	else
	{
		bItemTypeOkay = UMyLib::GetItemType(itemData) == m_FilterCategoryItem;
	}

	if (m_FilterCategoryEquip == EEquipSlotType::None)
	{
		bEquipTypeOkay = true;
	}
	else
	{
		bEquipTypeOkay = itemData.m_ItemType == m_FilterCategoryEquip;	
	}

	return bColorOkay && bItemTypeOkay && bEquipTypeOkay;
}

void UWidgetCraftPanel::ClearFilter()
{
	m_ItemFilter->ClearFilter();
	m_ColorFilter->ClearFilter();
	m_EquipFilter->ClearFilter();
	//
	m_FilterCategoryItem = m_ItemFilter->GetCrntItemType();
	m_FilterCategoryEquip = m_EquipFilter->GetCrntEquipType();
	m_FilterColorData = m_ColorFilter->GetCrntColor();
	//
	UpdateCraftablePanel();
}

void UWidgetCraftPanel::OnFilterItem()
{
	m_FilterCategoryItem = m_ItemFilter->GetCrntItemType();
	
	UpdateCraftablePanel();
}

void UWidgetCraftPanel::OnFilterEquip()
{
	m_FilterCategoryEquip = m_EquipFilter->GetCrntEquipType();
	
	UpdateCraftablePanel();
}

void UWidgetCraftPanel::OnFilterColor()
{
	m_FilterColorData = m_ColorFilter->GetCrntColor();

	UpdateCraftablePanel();
}