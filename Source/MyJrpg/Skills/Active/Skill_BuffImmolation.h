// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"
#include "Skill_BuffImmolation.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_BuffImmolation : public USkill_BuffBase
{
	GENERATED_BODY()
public:
	USkill_BuffImmolation(const FObjectInitializer& obj);
protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
protected:
	virtual void OnTick(float delta) override;
};
