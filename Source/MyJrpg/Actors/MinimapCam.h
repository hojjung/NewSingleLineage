// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyJrpg/MyLib.h"
#include "MinimapCam.generated.h"

UCLASS()
class MYJRPG_API AMinimapCam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinimapCam();


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	
protected:
	virtual void BeginPlay() override;

public:
	void AddTrackIcon(IFocusable* icon);
};
