// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUnitPawn.h"
#include "MyPlayerPawn.h"
#include "MyJrpg/DataTables/PetTable.h"
#include "PetPawn.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API APetPawn : public ABaseUnitPawn
{
	GENERATED_BODY()

public:
	APetPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY()
	AMyPlayerPawn* m_PlayerPawn;

	float m_fInitZ;

	float m_fMaxZ;

	float m_fMinZ;

	float m_fTimer;

	bool m_bIsMax;

	float m_fFloatingTime;

	float m_fFloatingZ;
public:
	void SetPetEntity(const FPetRow& pet_row);
	
protected:
	virtual void Tick(float DeltaSeconds) override;

	void AnimateFly(float DeltaSeconds);
};
