// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_Harvest.generated.h"

/**
 * 부채꼴로 범위공격, 피흡
 */
UCLASS()
class MYJRPG_API USkill_Harvest : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_Harvest();
	
protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
protected:
	virtual void OnSkillTriggered() override;
};
