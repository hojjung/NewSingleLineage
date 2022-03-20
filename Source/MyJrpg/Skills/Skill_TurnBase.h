// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill_BuffBase.h"
#include "Skill_TurnBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_TurnBase : public USkill_BuffBase
{
	GENERATED_BODY()
protected:
	TSoftObjectPtr<UUnitEntityAsset> m_TurnSk;

protected:
	virtual void EndSkillAnim() override;//is trigger

	virtual void EndDuration() override;
};
