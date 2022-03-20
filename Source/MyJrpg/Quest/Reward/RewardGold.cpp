// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardGold.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void URewardGold::ExecuteRewardReceive(int amount)
{
	UMyGameInstance::Get->m_CurrencyManager->AddGold(amount);
}
