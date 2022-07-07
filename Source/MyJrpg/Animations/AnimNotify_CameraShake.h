// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UMatineeCameraShake> m_ClassCamShake;
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
