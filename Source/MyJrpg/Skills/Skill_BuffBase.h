// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "Skill_BuffBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_BuffBase : public USkillBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;

	virtual bool CanUseSkill() override;
public:
	float GetDuration() const;

	virtual void OnTick(float delta); 

	virtual void EndDuration();
};
