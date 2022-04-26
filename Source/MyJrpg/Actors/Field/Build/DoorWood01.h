// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureActor.h"
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
	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_DoorTrigger;
	UPROPERTY()
	TArray<UMaterialInterface*> m_AryDoorMats;

	bool m_bIsDoorOpen;

	float m_fRot;
protected:
	virtual void BeginPlay() override;
	
	virtual void SetMat(UMaterialInterface* mat) override;

	virtual void ConfirmBuild() override;
	
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

public:
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult & SweepResult);
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};



