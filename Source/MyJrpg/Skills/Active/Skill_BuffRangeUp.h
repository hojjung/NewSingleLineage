// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"
#include "Skill_BuffRangeUp.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_BuffRangeUp : public USkill_BuffBase
{
	GENERATED_BODY()
protected:
	virtual void OnSkillTriggered() override;

	virtual void EndDuration() override;
};
