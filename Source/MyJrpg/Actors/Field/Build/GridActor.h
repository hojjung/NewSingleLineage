// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GridActor.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AGridActor : public AStaticMeshActor
{
	GENERATED_BODY()

protected:
	AGridActor();

public:
	void Show();

	void Hide();
};