// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_FrozenFeet.generated.h"

/**
 * 주변 발 얼리기
 */
UCLASS()
class MYJRPG_API USkill_FrozenFeet : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_FrozenFeet();

protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
	
protected:
	virtual void OnSkillTriggered() override;

	virtual bool CanUseSkill() override;
};
