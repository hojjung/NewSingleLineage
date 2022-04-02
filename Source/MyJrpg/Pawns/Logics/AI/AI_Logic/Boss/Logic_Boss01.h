// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Pawns/Logics/AI/AI_Logic/AI_LogicBase.h"
#include "MyJrpg/Pawns/Logics/AI/AI_Logic/Logic_NPCDefault.h"
#include "MyJrpg/Pawns/Logics/MonsterSkill/MobSkillBase.h"
#include "MyJrpg/Pawns/Logics/MonsterSkill/MobSkill_CircleSelfAOE.h"
#include "Logic_Boss01.generated.h"

class ADecalActor;
/**
 * //Material'/Game/03_VisualEffect/Decals/M_EnemySkillIndicator.M_EnemySkillIndicator'
 */
UCLASS()
class MYJRPG_API ULogic_Boss01 : public ULogic_NPCDefault
{
	GENERATED_BODY()

public:
	ULogic_Boss01();

protected:
	float m_fSkillTimer;

	const float m_fSkillUseCD = 3;

protected:
	UPROPERTY()
	UMaterialInterface* m_Mat;
	UPROPERTY()
	UMobSkill_CircleSelfAOE* m_Skill01;
	
protected:
	virtual void Init(ACombatUnitPawn* owner) override;
	
	virtual void OnCombat() override;

	virtual void CheckSetState() override;
	
};
