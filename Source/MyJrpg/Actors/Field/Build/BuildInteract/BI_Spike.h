// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildInteractBase.h"
#include "BI_Spike.generated.h"

/**
 * 몬스터에게 주변에 데미지
 */
UCLASS()
class MYJRPG_API UBI_Spike : public UBuildInteractBase
{
	GENERATED_BODY()

public:
	virtual void Init(const TArray<FString>& variable, UInventory* inven) override;

	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
