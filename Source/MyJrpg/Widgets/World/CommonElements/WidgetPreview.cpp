#include "WidgetPreview.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetPreview::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	m_PreviewImage->SetBrushFromMaterial(m_PreviewMat);
}

FReply UWidgetPreview::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	UMyGameInstance::Get->m_AvatarManager->SetIsTouched(true);
	
	return FReply::Handled();
}

FReply UWidgetPreview::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	UMyGameInstance::Get->m_AvatarManager->SetIsTouched(false);

	return FReply::Handled();
}

FReply UWidgetPreview::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	
	float DeltaX = InGestureEvent.GetCursorDelta().X;

	if(DeltaX != 0.f)
	{
		UMyGameInstance::Get->m_AvatarManager->RotatePawn(DeltaX);
	}

	return FReply::Handled();
}


