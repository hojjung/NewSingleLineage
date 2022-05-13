// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureActor.h"
#include "DoorActor.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ADoorActor : public AStructureActor
{
	GENERATED_BODY()
	
public:
	ADoorActor();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshDoor;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* m_DoorTrigger;

	bool m_bIsDoorOpen;

	float m_fRot;
protected:
	virtual void SetBuildData(const FBuildDataRow& data) override;
	
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

public:
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult & SweepResult);
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
