// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardExp.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void URewardExp::ExecuteRewardReceive(int amount)
{
	UMyGameInstance::Get->m_PlayerStatManager->AddExp(amount);
}
