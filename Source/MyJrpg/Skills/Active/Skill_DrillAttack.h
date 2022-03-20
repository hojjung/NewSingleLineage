// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_DrillAttack.generated.h"

/**
 * 전방에 칼질난무
 */
UCLASS()
class MYJRPG_API USkill_DrillAttack : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;
};
