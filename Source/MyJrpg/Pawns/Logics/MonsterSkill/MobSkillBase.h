// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "UObject/NoExportTypes.h"
#include "MobSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMobSkillBase : public UObject
{
	GENERATED_BODY()

public:
	UMobSkillBase();

protected:
	UPROPERTY()
	UAnimMontage* m_SkillMotion;
	UPROPERTY()
	UMaterialInterface* m_Mat;
	UPROPERTY()
	ADecalActor* m_Decal;
	UPROPERTY()
	ACombatUnitPawn* m_Owner;

	FName m_Id;

	FDelegateHandle m_SkillHandle;

	FTimerHandle m_SkillEnd;
	
	bool m_bIsSkillUsing;

	float m_fRadius;

	float m_fDamage;
	
protected:
	void CheckSkillTriggered(FName id);
	
	void SpawnDecal();
	
	void PlayAnimMontage();
	void KillDecal();

	bool TraceBox(TArray<AActor*>& outHits, float range, FVector boxHalfSize, TSubclassOf<ACombatUnitPawn> classFilter = ACombatUnitPawn::StaticClass());
	
	bool TraceSphere(TArray<AActor*>& outHits, float range, float radius, TSubclassOf<ACombatUnitPawn> classFilter = ACombatUnitPawn::StaticClass());
	
	bool TraceSphere(TArray<AActor*>& outHits, FVector start, float radius,TSubclassOf<ACombatUnitPawn> classFilter = ACombatUnitPawn::StaticClass());

	void EndSkillAnim();
	
	virtual void OnTriggerSkill();
	
public:
	virtual	void Init(ACombatUnitPawn* owner);
	
	virtual void UseSkill();
	
	
	virtual bool IsUsing();
};
