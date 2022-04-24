// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/StructureActor.h"
#include "DoorWood01.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ADoorWood01 : public AStructureActor
{
	GENERATED_BODY()

public:
	ADoorWood01();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshDoor;
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryDoorMats;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetMat(UMaterialInterface* mat) override;

	virtual void ConfirmBuild() override;
};
