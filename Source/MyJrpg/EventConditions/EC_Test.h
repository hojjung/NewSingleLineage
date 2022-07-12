// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventConditionBase.h"
#include "EC_Test.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEC_Test : public UEventConditionBase
{
	GENERATED_BODY()


public:
	virtual bool TryUnlockEvent() override;
};
