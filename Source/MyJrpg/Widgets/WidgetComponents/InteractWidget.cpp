// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"

void UInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

FReply UInteractWidget::NativeOnTouchStarted(const FGeometry& in_geometry, const FPointerEvent& in_gesture_event)
{
	Super::NativeOnTouchStarted(in_geometry, in_gesture_event);

	m_OnInteract.Broadcast();
	
	return FReply::Handled(); 
}

FReply UInteractWidget::NativeOnMouseButtonDown(const FGeometry& in_geometry, const FPointerEvent& in_mouse_event)
{
	Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

	m_OnInteract.Broadcast();

	return FReply::Handled();
}
