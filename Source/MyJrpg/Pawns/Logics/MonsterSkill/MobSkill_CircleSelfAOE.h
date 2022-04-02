// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobSkillBase.h"
#include "UObject/NoExportTypes.h"
#include "MobSkill_CircleSelfAOE.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMobSkill_CircleSelfAOE : public UMobSkillBase
{
	GENERATED_BODY()

public:
	UMobSkill_CircleSelfAOE();
	
public:
	virtual void UseSkill() override;

protected:
	virtual void OnTriggerSkill() override;
};
