// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActorBase.h"
#include "NpcActor.h"
#include "ShopKeeper.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API AShopKeeper : public ANpcActor
{
	GENERATED_BODY()
	
public:	
	AShopKeeper();

protected:
	virtual void OnInteract() override;
};
