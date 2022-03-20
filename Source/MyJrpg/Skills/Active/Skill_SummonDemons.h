// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "MyJrpg/Skills/Skill_SummonBase.h"
#include "Skill_SummonDemons.generated.h"

/**
 * 악마 군단을 소환
 */
UCLASS()
class MYJRPG_API USkill_SummonDemons : public USkill_SummonBase
{
	GENERATED_BODY()

public:
   	USkill_SummonDemons(const FObjectInitializer& obj);

protected:
	virtual void OnSkillTriggered() override;
};
