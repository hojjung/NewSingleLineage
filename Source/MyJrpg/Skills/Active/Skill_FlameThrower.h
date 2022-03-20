// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_FlameThrower.generated.h"

/**
 * 전방에 화염방사
 */
UCLASS()
class MYJRPG_API USkill_FlameThrower : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;
};
