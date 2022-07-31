// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sensor_LogicBase.h"
#include "Sensor_ForgetTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USensor_ForgetTarget : public USensor_LogicBase
{
	GENERATED_BODY()

	
public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);
	
protected:
	FVector m_LastSeenLocation;
	
protected:
	virtual void Init(ACombatUnitPawn* owner) override;

	float m_fCurrentTargetDist;

	virtual void UpdateAISensing() override;

	virtual bool CheckDistAndAngle(const ACombatUnitPawn* Other) override;
};
