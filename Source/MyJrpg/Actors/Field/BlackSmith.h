// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NpcActor.h"
#include "BlackSmith.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ABlackSmith : public ANpcActor
{
	GENERATED_BODY()
	
public:	
	ABlackSmith();
protected:
	virtual void OnInteract() override;	
};
