// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "TreeBase.generated.h"

class UGatherAsset;
struct FGatherDataRow;

UCLASS()
class MYJRPG_API ATreeBase : public AActor, public IFocusable
{
	GENERATED_BODY()

public:
	ATreeBase();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Character)
	UCapsuleComponent* m_Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_ShadowMeshComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshTree;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshTrunk;
	UPROPERTY()
	AMyPlayerPawn* m_Player;

	int m_nTreeHp;

	TSoftObjectPtr<UGatherAsset> m_GatherAsset;

	bool m_bUsePhysics;

private:
	FFloatCurve m_CurveDeathAnim;

	float m_fDeathAnimDurationMax;

	float m_fDeathAnimDurationTimer;

private:
	void CreateSetDeathCurve(float fullLength);

	void StartDeathEffectMaterial(float duration);

	void SetDeathEffectMaterial(float deltaTime);
	
protected:
	virtual void OnInteract() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void SetEntity(const FGatherDataRow& data, AMyPlayerPawn* pl);
	
	virtual void OnTakeChopping();

	virtual void OnHarvestMotionDone();

	virtual void OnGatherDone();

	virtual float GetBoundHalfHeight() override;
};
