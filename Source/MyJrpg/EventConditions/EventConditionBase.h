// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventConditionBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEventConditionBase : public UObject
{
	GENERATED_BODY()

public:
	virtual  bool TryUnlockEvent() { return false; };
};
