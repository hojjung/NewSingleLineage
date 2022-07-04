// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStatHealth.h"

void UWidgetStatHealth::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_Hunger->ShowNumber(m_bShowNumber);

	if(m_bShowNumber)
	{
		m_HungerSize->SetWidthOverride(150.f);
	}
	else
	{
		m_HungerSize->SetWidthOverride(75.0f);
	}
}

void UWidgetStatHealth::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Hunger->ShowNumber(m_bShowNumber);

	if(m_bShowNumber)
	{
		m_HungerSize->SetWidthOverride(150.f);
	}
	else
	{
		m_HungerSize->SetWidthOverride(75.0f);
	}
}
