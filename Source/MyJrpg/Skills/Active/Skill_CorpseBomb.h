// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_CorpseBomb.generated.h"

/*
 * 한 몬스터를 처치하면,그몬스터 주변에서 다시 시전됨
 */
UCLASS()
class MYJRPG_API USkill_CorpseBomb : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_CorpseBomb(const FObjectInitializer& obj);

protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
	
protected:
	virtual void OnSkillTriggered() override;

	void ExecuteCorpseBomb(ACombatUnitPawn* pawn);
};
