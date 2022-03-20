// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_Basher.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_Basher : public USkillBase
{
	GENERATED_BODY()
	
	virtual void OnSkillTriggered() override;
};
