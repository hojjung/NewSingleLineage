// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_SilenceCut.generated.h"

/**
 * 상대의 뒤로 이동하며 데미지,몇초간 침묵
 */
UCLASS()
class MYJRPG_API USkill_SilenceCut : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;
};
