// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSkinPanel.h"

void UWidgetSkinPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetSkinPanel::OnClose);
}

void UWidgetSkinPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
