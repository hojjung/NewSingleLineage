// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_Whirlwind.generated.h"

/**
 * 몇초간 휠윈드
 * 버프로 하되 애니매이션은 계속 작동해야함
 * 애니자체를 한 모션으로 만들면되는거?
 * 공속이 빠르면 빨리 끝나나?
 * 버프로 안하고 트리거를 여러번?
 */
UCLASS()
class MYJRPG_API USkill_Whirlwind : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;

	virtual bool CanUseSkill() override;
};
