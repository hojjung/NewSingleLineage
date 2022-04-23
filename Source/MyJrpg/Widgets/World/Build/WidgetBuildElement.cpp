#include "WidgetBuildElement.h"

void UWidgetBuildElement::Init(const FBuildDataRow& data)
{
	m_DataRow = &data;

		
}

void UWidgetBuildElement::MyFocus()
{
	
}

void UWidgetBuildElement::MyUnFocus()
{
	
}

FReply UWidgetBuildElement::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);
	
	return FReply::Handled();
}

FReply UWidgetBuildElement::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	m_OnClick.ExecuteIfBound(this, *m_DataRow);

	return FReply::Handled();
}