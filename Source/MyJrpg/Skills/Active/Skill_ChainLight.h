// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_ChainLight.generated.h"

/**
 * 5마리까지?
 * 나에게 가장 가까운 몹하나를 선점
 * 그몹에게 가까운몹 다시 선점
 * 이모든곳에 이펙트가 연결되야함
 */
UCLASS()
class MYJRPG_API USkill_ChainLight : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_ChainLight();
	
protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
	UPROPERTY(Transient)
	TArray<UParticleSystemComponent*> m_ArySpawnedLaser;
	UPROPERTY(Transient)
	TSet<ACombatUnitPawn*> m_Hitten;
	
protected:
	virtual void OnSkillTriggered() override;

	virtual void EndSkillAnim() override;

	ACombatUnitPawn* ExecuteChainLight(FVector&& start, float range);
};
//
