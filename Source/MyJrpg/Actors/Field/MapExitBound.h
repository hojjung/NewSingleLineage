// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/Pawns/Components/MinimapIconComp.h"
#include "MapExitBound.generated.h"

UCLASS()
class MYJRPG_API AMapExitBound : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapExitBound();
	
protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* m_CollBox;
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* m_TextRender;
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* m_Decal;
	UPROPERTY(VisibleAnywhere)
	UMinimapIconComp* m_IconMeshComp;
	UPROPERTY()
	UTexture2D* m_Icon;

	FTimerHandle m_Timer;
	
protected:
	virtual void BeginPlay() override;

	void MoveToMapLevel();

	void SetMinimap();

public:
	UFUNCTION()
	void OnTriggerStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
