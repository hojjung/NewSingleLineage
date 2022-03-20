// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_BuffCri.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_BuffCri : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;

	virtual void EndSkillAnim() override;
};
