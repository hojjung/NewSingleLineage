// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "MapExitBound.generated.h"

UCLASS()
class MYJRPG_API AMapExitBound : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapExitBound();
	
protected:
	UPROPERTY()
	UBoxComponent* m_CollBox;
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* m_TextRender;
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* m_Decal;
	
protected:
	virtual void BeginPlay() override;

	void MoveToMapLevel();

public:
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
