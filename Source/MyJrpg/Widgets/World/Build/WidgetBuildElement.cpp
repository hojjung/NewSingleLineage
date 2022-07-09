#include "WidgetBuildElement.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftCostElement.h"

void UWidgetBuildElement::Init(const FBuildDataRow& data)
{
	m_DataRow = &data;

	MyUnFocus();

	m_ImgIcon->SetBrushFromSoftTexture(data.m_Icon);

	m_TextCnt->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBuildElement::SetStackCount(int cnt)
{
	m_TextCnt->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextCnt->SetText(FText::AsNumber(cnt));
}

void UWidgetBuildElement::MyFocus()
{
	m_ImgFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetBuildElement::MyUnFocus()
{
	m_ImgFocus->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWidgetBuildElement::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);
	
	return FReply::Handled();
}

FReply UWidgetBuildElement::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);

	return FReply::Handled();
}

void UWidgetBuildElement::UpdateCost()
{
	for(UWidgetCraftCostElement* CraftCost : m_CraftCost)
	{
		CraftCost->UpdateCostAmount();
	}
}

void UWidgetBuildElement::CreateCostWidgets(const TArray<FCraftItemCost>& costData)
{
	m_VertCost->ClearChildren();

	m_CraftCost.Reset();
	
	for(const FCraftItemCost& CraftData : costData)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCostElement);

		SelectButton->BoundStackDefaultStackFunPtr();
		
		SelectButton->SetCraftCost(CraftData);

		m_VertCost->AddChildToVerticalBox(SelectButton);

		m_CraftCost.Add(SelectButton);
		
		SelectButton->SetPadding(FMargin(0,0,0,0));

		SelectButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
