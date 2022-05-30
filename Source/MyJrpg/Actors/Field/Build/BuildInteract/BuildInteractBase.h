// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildInteractBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBuildInteractBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(const FString& variable) {};
	
	virtual bool IsEraseable() { return true;};

	virtual void OnInteract() {};
};
