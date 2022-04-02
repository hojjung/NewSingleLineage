// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sensor_LogicBase.h"
#include "Sensor_AlwaysTargetPl.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USensor_AlwaysTargetPl : public USensor_LogicBase
{
	GENERATED_BODY()

	virtual void UpdateAISensing() override;
};
