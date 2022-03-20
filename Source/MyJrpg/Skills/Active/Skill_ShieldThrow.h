// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_ShieldThrow.generated.h"

/**
 * 체인 라이트닝 처럼 방패던지기,떨어진 방패를 주우면 쿨초기화
 */
UCLASS()
class MYJRPG_API USkill_ShieldThrow : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_ShieldThrow(const FObjectInitializer& obj);

protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
	UPROPERTY(Transient)
	TSet<ACombatUnitPawn*> m_Hitten;
protected:
	virtual void OnSkillTriggered() override;

	ACombatUnitPawn* Shoot(const FVector& start);
};
