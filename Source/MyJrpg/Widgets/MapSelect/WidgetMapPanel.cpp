#include "WidgetMapPanel.h"

void UWidgetMapPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> AryWidgets = m_CanvasMap->GetAllChildren();

	for(UWidget* ChildWidget :  AryWidgets)
	{
		UWidgetMapBtn* Btn = Cast<UWidgetMapBtn>(ChildWidget);

		if(!Btn)
			continue;

		Btn->m_OnClick.BindUObject(this, &UWidgetMapPanel::OnClick);
	}

	m_ZoneSelect->SetVisibility(ESlateVisibility::Collapsed);

	m_PanelSlot = Cast<UCanvasPanelSlot>(m_CanvasMap->Slot);
}

void UWidgetMapPanel::OnClick(const FName& zoneID)
{
	m_ZoneSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FZoneDataRow* ZoneData = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(zoneID,"");

	m_ZoneSelect->Init(*ZoneData);
}

FReply UWidgetMapPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetMapPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	return FReply::Handled();
}

FReply UWidgetMapPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	FVector2D OutCanvasSize = InGeometry.GetLocalSize() / 2.f;

	FVector2D PanelSize = m_PanelSlot->GetSize() / 4.f;
	
	float RateX =  PanelSize.X / OutCanvasSize.X;

	float RateY =  PanelSize.Y / OutCanvasSize.Y;

	PanelSize.X *= RateX;

	PanelSize.Y *= RateY;

	FVector2D Pos = m_PanelSlot->GetPosition();

	const FVector2D& Delta = InGestureEvent.GetCursorDelta();

	FVector2D ResultPos = Pos + (Delta * 2.f);

	ResultPos.X = FMath::Clamp<float>(ResultPos.X, -PanelSize.X, PanelSize.X);

	ResultPos.Y = FMath::Clamp<float>(ResultPos.Y, -PanelSize.Y, PanelSize.Y);

	m_PanelSlot->SetPosition(ResultPos);

	return FReply::Handled();
}


