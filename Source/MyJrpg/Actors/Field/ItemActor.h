// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "ItemActor.generated.h"

UCLASS()
class MYJRPG_API AItemActor : public AActor, public IFocusable
{
	GENERATED_BODY()
	
public:	
	AItemActor();

protected:
	FName m_OwnerID;

protected:
	virtual void BeginPlay() override;

public:	
	void Obtain();

	bool HasOwnerTeamID() const;
};

