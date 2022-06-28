// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MinimapCompParent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYJRPG_API UMinimapCompParent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMinimapCompParent();

protected:
	FVector m_RelativeSocketLocation;

	FQuat m_RelativeSocketRotation;

	FRotator m_RotOffset;
	
protected:
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetRotationOffset(FRotator rot);
};
