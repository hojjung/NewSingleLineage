// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBasePanel.h"

void UWidgetBasePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetBasePanel::ClosePanel);

	m_BtnClose->IsFocusable = false;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBasePanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetBasePanel::OpenPanel()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
