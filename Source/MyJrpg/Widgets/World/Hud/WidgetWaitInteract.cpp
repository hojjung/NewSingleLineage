// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetWaitInteract.h"

void UWidgetWaitInteract::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Bar->SetVisibility(ESlateVisibility::Collapsed);

	m_fMaxTimer = -1;
}

void UWidgetWaitInteract::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_fMaxTimer <= -1)
	{
		return;
	}
	m_fTimer += InDeltaTime;

	m_Bar->SetPercent(m_fTimer / m_fMaxTimer);

	if(m_fTimer >= m_fMaxTimer)
	{
		HideInteract();
	}
}

void UWidgetWaitInteract::ShowInteract(float time)
{
	m_Bar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_fMaxTimer = time;
	
	m_fTimer = 0;
}

void UWidgetWaitInteract::HideInteract()
{
	m_Bar->SetVisibility(ESlateVisibility::Collapsed);

	m_fMaxTimer = -1;
}