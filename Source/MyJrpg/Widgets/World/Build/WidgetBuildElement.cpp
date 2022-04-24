#include "WidgetBuildElement.h"

#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftCostElement.h"

void UWidgetBuildElement::Init(const FBuildDataRow& data)
{
	m_DataRow = &data;

	MyUnFocus();

	m_ImgIcon->SetBrushFromSoftTexture(data.m_Icon);

	CreateCostWidgets(data.m_AryCostItem);
}

void UWidgetBuildElement::MyFocus()
{
	m_ImgFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetBuildElement::MyUnFocus()
{
	m_ImgFocus->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWidgetBuildElement::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);
	
	return FReply::Handled();
}

FReply UWidgetBuildElement::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);

	return FReply::Handled();
}

void UWidgetBuildElement::CreateCostWidgets(const TArray<FCraftItemCost>& costData)
{
	m_VertCost->ClearChildren();
	
	for(const FCraftItemCost& CraftData : costData)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCostElement);

		SelectButton->SetCraftCost(CraftData);

		m_VertCost->AddChildToVerticalBox(SelectButton);
		
		SelectButton->SetPadding(FMargin(0,0,0,0));
	}
}