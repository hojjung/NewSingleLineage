// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneMoveManager.generated.h"

/**
 *  스태미나 충전 및 소모
 *  달리기 코스트 계산
 *  실제 달리기 타임 계산
 */
UCLASS()
class MYJRPG_API UZoneMoveManager : public UObject
{
	GENERATED_BODY()

protected:
	int m_nZoneStamina;

public:
	void Init();

	void Tick(float deltaTime);

	
};
