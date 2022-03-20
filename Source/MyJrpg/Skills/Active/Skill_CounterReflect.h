// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_CounterReflect.generated.h"

/**
 * 무적,몇초동안 받은 피해 * 1.5 배를 범위공격으로 되돌려줌
 */
UCLASS()
class MYJRPG_API USkill_CounterReflect : public USkillBase
{
	GENERATED_BODY()

protected:
	FDelegateHandle m_DealHandle;
	UPROPERTY()
	float m_fDmgTotal;
	
protected:
	virtual void OnSkillTriggered() override;
	
	virtual void EndSkillAnim() override;

	void OnPlayerTookDmg(float dmg);
};
