// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetExpGauge.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetExpGauge::SetProgressValue(float v)
{
	m_ExpBar->SetPercent(v);
}

bool UWidgetExpGauge::Initialize()
{
	bool RR =  Super::Initialize();
	
	if(!UMyGameInstance::Get->IsValidLowLevel())
	{
		return false;
	}

	SetProgressValue(0);
	UpdateExpBar();
	return RR;
}

void UWidgetExpGauge::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UMyGameInstance::Get->m_PlayerStatManager->m_OnExpChanged.AddUObject(this,&UWidgetExpGauge::UpdateExpBar);
	//UpdateExpBar();
}

void UWidgetExpGauge::UpdateExpBar()
{
	float CExp = UMyGameInstance::Get->m_PlayerStatManager->GetCExp();

	float MExp = UMyGameInstance::Get->m_PlayerStatManager->GetMaxExp();

	SetProgressValue(CExp / MExp);
}

