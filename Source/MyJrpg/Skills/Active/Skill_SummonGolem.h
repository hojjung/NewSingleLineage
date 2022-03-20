// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/Skill_SummonBase.h"
#include "Skill_SummonGolem.generated.h"

/**
 * 골렘 한마리 소환
 */
UCLASS()
class MYJRPG_API USkill_SummonGolem : public USkill_SummonBase
{
	GENERATED_BODY()

public:
	USkill_SummonGolem(const FObjectInitializer& obj);
	
};
