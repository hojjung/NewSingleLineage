// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyMovement.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "UObject/NoExportTypes.h"
#include "MyFlockSteering.generated.h"

class AMonsterPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API UMyFlockSteering : public UMyMovement
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<AMonsterPawn*> m_NearMobs;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void ApplyControlInputToVelocity(float DeltaTime) override;

protected:
	FVector GetBoidDelta(FVector inputDelta);

	virtual void TrySnapFloor() override;
};
