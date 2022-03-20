// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "MyJrpg/Pawns/SummonUnitPawn.h"
#include "Skill_SummonBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_SummonBase : public USkillBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FName m_SummonUnitId;
	
protected:
	virtual void OnSkillTriggered() override;

	bool SpawnUnit();
};
