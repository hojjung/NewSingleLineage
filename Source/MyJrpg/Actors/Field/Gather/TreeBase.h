// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/InteractActorBase.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "TreeBase.generated.h"

/**
 *	StaticMesh'/Game/08_EnvironmentMesh/ARPGEnvironment/Meshes/Natures/SM_Aspen_Tree_01.SM_Aspen_Tree_01'
 * StaticMesh'/Game/08_EnvironmentMesh/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Tree_Stump01.SM_Plains_Tree_Stump01'
 *
 * old: StaticMesh'/Game/08_EnvironmentMesh/InfinityBladeFireLands/Environments/Fire/Env_Fire_OddsnEnds/StaticMesh/SM_Env_Fire_DeadTree.SM_Env_Fire_DeadTree'
 * oldtrunk:StaticMesh'/Game/08_EnvironmentMesh/InfinityBladeFireLands/Environments/Fire/Env_Fire_Rocky/StaticMesh/SM_Fire_Tree_Stump_3.SM_Fire_Tree_Stump_3'
 *
 * wood chop: AnimMontage'/Game/09_SharedAnimations/Player/Axe_Swing_Down_Anim_Montage.Axe_Swing_Down_Anim_Montage'
 * rock anim : AnimMontage'/Game/09_SharedAnimations/Player/Pick_Swing_Med_Anim_Montage.Pick_Swing_Med_Anim_Montage'
 *
 * SoundWave'/Game/Sound/Tree_Falling_4.Tree_Falling_4'
 */
UCLASS()
class MYJRPG_API ATreeBase : public AInteractActorBase
{
	GENERATED_BODY()

public:
	ATreeBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshTree;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshTrunk;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* m_Motion;
	UPROPERTY()
	AMyPlayerPawn* m_Player;
	UPROPERTY()
	USoundBase* m_Sound;

	int m_nTreeHp;

private:
	FFloatCurve m_CurveDeathAnim;

	float m_fDeathAnimDurationMax;

	float m_fDeathAnimDurationTimer;

private:
	void CreateSetDeathCurve(float fullLength);

	void StartDeathEffectMaterial(float duration);

	void SetDeathEffectMaterial(float deltaTime);
	
protected:
	virtual void BeginPlay() override;

	virtual void OnInteract() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void OnTakeChopping();

	virtual void OnHarvestMotionDone();

	virtual void OnGatherDone();
};
