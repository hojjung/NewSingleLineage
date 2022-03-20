// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/Skill_TurnBase.h"
#include "Skill_TurnDemon.generated.h"

/**
 * 악마로 탈바꿈
 * 유저 변신 스킨이랑 안겹처야한다
 * 변신한동안 바꾸면 어떻게 됨?
 */
UCLASS()
class MYJRPG_API USkill_TurnDemon : public USkill_TurnBase
{
	GENERATED_BODY()

public:
	USkill_TurnDemon(const FObjectInitializer& obj);

protected:
	virtual void EndSkillAnim() override;

	virtual void EndDuration() override;
};
