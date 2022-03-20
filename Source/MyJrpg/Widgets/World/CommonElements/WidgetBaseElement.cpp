#include "WidgetBaseElement.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"

void UWidgetBaseElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_bIsHoldable = true;

	m_bIsFocusable = true;
	
	Clear();

	SetMyUnFocus();
}

void UWidgetBaseElement::Clear()
{
	m_ImgGlow->SetBrushFromTexture(m_DefaultGlow);
	
	m_ImgItemIcon->SetVisibility(ESlateVisibility::Collapsed);

	m_HoldingBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::SetHoldable(bool isActive)
{
	m_bIsHoldable = isActive;
}

FReply UWidgetBaseElement::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}
//
FReply UWidgetBaseElement::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if(!m_bIsHoldable)
	{
		return FReply::Handled();
	}
	
	m_bHolding = true;

	m_fTimer = 0;

	return FReply::Handled();
}

FReply UWidgetBaseElement::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (InGestureEvent.GetCursorDelta().SizeSquared()>100.0f)
	{
		EndHolding();
	}

	return FReply::Handled();
}

FReply UWidgetBaseElement::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	EndHolding();

	if(m_bIsFocusable)
	{
		OnClick();
	}

	return FReply::Handled();
}

void UWidgetBaseElement::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	EndHolding();
}

void UWidgetBaseElement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_bHolding ||!m_bIsHoldable) //포커싱이 이미되버렸으면 안함//||m_OverlayFocus->IsVisible()
	{
		return;
	}

	m_fTimer += InDeltaTime;

	if (m_fTimer >= 0.2f)
	{
		m_HoldingBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (m_fTimer >= 0.7f)
	{
		OnHoldingComplete();

		EndHolding();
	}

	m_HoldingBar->SetPercent(m_fTimer / 0.7f);
}

void UWidgetBaseElement::EndHolding()
{
	m_bHolding = false;

	m_HoldingBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::SetIcon(TSoftObjectPtr<UTexture2D> t)
{
	m_ImgItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_ImgItemIcon->SetBrushFromSoftTexture(t);
}

void UWidgetBaseElement::SetGlowColor(const FColorDataHandle& color)
{
	const FColorDataRow* ColorDataRow = color.GetRow<FColorDataRow>("");
	
	m_ImgGlow->SetBrushFromTexture(ColorDataRow->m_GlowTexture);
}

bool UWidgetBaseElement::IsFocused()
{
	return m_OverlayFocus->IsVisible(); 
}

void UWidgetBaseElement::OnClick()
{
	m_OnClick.Broadcast();
}

void UWidgetBaseElement::SetMyFocus()
{
	m_OverlayFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetBaseElement::SetMyUnFocus()
{
	m_OverlayFocus->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBaseElement::OnHoldingComplete()
{
	m_OnHold.Broadcast();
}

void UWidgetBaseElement::SetFocusable(bool isActive)
{
	m_bIsFocusable = isActive;
}

bool UWidgetBaseElement::IsMyFocused() const
{
	return m_OverlayFocus->IsVisible();
}

