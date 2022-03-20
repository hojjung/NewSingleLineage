// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewardLogicBase.h"
#include "RewardGold.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API URewardGold : public URewardLogicBase
{
	GENERATED_BODY()

	virtual void ExecuteRewardReceive(int amount) override;
};
