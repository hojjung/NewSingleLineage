// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "IndoorBase.generated.h"

UCLASS()
class MYJRPG_API AIndoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AIndoorBase();//관통 메테리얼은 직접

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* m_BoxIndoor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UStaticMeshComponent*> m_AryRoofs;
protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
