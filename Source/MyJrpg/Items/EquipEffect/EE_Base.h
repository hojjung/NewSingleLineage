// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EE_Base.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UEE_Base : public UObject
{
	GENERATED_BODY()

public:
	virtual void Equip() {};
	
	virtual void UnEquip() {};
};
