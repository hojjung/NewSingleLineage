// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatUnitPawn.h"
#include "SummonUnitPawn.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ASummonUnitPawn : public ACombatUnitPawn
{
	GENERATED_BODY()

public:
	void SetLifeTime(float lifetime);
};
