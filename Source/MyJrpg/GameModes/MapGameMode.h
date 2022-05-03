// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AMapGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMapGameMode();

protected:
	virtual void BeginPlay() override;
	
};
