// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RewardLogicBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MYJRPG_API URewardLogicBase : public UObject
{
	GENERATED_BODY()
public:
	virtual  void ExecuteRewardReceive(int amount) {};
};
