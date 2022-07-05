// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinimapIconComp.generated.h"

/**
 * PrimaryComponentTick.bCanEverTick = false;
 */
UCLASS()
class MYJRPG_API UMinimapIconComp : public USceneComponent
{
	GENERATED_BODY()

public:
	UMinimapIconComp(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_IconMeshComp;
	
	FVector m_RelativeSocketLocation;

	FQuat m_RelativeSocketRotation;

	FRotator m_RotOffset;

	bool m_bUseRot;
	
protected:
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetRotationOffset(FRotator rot);

	UStaticMeshComponent* GetMeshComp();

	void SetIcon(UTexture2D* wantIcon);

	void SetLayerHeight(float h);
};
