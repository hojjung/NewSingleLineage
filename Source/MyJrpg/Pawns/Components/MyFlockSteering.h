// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyMovement.h"
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
	TArray<AActor*> m_AryIgnoreActors;
	
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	
protected:
	virtual void BeginPlay() override;
	
	bool GetNeighborPawns(TArray<AActor*>& aryOut);

	FVector GetFlockDir();

	virtual void ApplyControlInputToVelocity(float DeltaTime) override;

	virtual void NotifyBumpedPawn(APawn* BumpedPawn) override;
};
