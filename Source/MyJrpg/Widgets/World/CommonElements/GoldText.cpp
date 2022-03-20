// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldText.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UGoldText::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UMyGameInstance::Get->m_CurrencyManager->m_GoldChanged.AddUObject(this,&UGoldText::UpdateGoldText);

	UpdateGoldText();
}

void UGoldText::UpdateGoldText()
{
	int Gold = UMyGameInstance::Get->m_CurrencyManager->GetGold();
	
	m_ImgText->SetText(FText::AsNumber(Gold));
}
