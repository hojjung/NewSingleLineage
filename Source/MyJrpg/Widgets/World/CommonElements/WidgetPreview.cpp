#include "WidgetPreview.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPreview::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	m_PreviewImage->SetBrushFromMaterial(m_PreviewMat);
}

void UWidgetPreview::NativeDestruct()
{
	Super::NativeDestruct();
	m_PreviewProxy = nullptr;
}

void UWidgetPreview::Init(UObject* proxyObj)
{
	m_PreviewProxy.SetObject(proxyObj);
	m_PreviewProxy.SetInterface(Cast<IPreviewProxy>(proxyObj));
}

FReply UWidgetPreview::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	m_PreviewProxy->SetIsTouched(true);
	
	return FReply::Handled();
}

FReply UWidgetPreview::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	m_PreviewProxy->SetIsTouched(false);

	return FReply::Handled();
}

void UWidgetPreview::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	m_PreviewProxy->SetIsTouched(false);
}

FReply UWidgetPreview::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	float DeltaX = InGestureEvent.GetCursorDelta().X;

	if(DeltaX != 0.f)
	{
		m_PreviewProxy->RotatePawn(DeltaX);
	}

	return FReply::Handled();
}


