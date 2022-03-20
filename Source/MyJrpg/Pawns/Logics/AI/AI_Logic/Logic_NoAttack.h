// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_LogicBase.h"
#include "Logic_NoAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ULogic_NoAttack : public UAI_LogicBase
{
	GENERATED_BODY()

protected:
	virtual void Init(ACombatUnitPawn* owner) override;
};


