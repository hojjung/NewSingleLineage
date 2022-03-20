// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActorBase.h"
#include "NpcActor.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ANpcActor : public AInteractActorBase
{
	GENERATED_BODY()
	
public:	
	ANpcActor();

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* m_MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	UPROPERTY()
	class  UAnimationAsset* m_IdleAnim;

protected:
	virtual void BeginPlay() override;
};
