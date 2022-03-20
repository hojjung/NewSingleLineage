// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_DeathLotus.generated.h"

/**
 * 빙글빙글돌면서 범위 공격
 * 공격하는 대상에게 투사체 이펙트를 날려주기
 */
UCLASS()
class MYJRPG_API USkill_DeathLotus : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_DeathLotus(const FObjectInitializer& obj);

protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
protected:
	virtual void OnSkillTriggered() override;

	void Shoot(AMonsterPawn* mob, const FVector& start);
};
