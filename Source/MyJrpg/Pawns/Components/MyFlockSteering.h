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
	TSet<ACombatUnitPawn*> m_SetIgnoreSelf;
	UPROPERTY()
	TArray<ACombatUnitPawn*> m_NearMobs;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void ApplyControlInputToVelocity(float DeltaTime) override;

	virtual void NotifyBumpedPawn(APawn* BumpedPawn) override;

protected:
	FVector GetFlockDir();
	
	FVector CalculateSeparationVector();
};
