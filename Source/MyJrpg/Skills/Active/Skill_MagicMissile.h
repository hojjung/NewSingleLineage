// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_MagicMissile.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USkill_MagicMissile : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_MagicMissile();

protected:
	UPROPERTY()
	UParticleSystem* m_BaseBullet; 
	UPROPERTY()
	UBulletPool* m_Pool;
	
protected:
	virtual void SetSkill(FName id, const FSkillDataRow& dataRow, int skillLevel) override;
	void ShootBullet();

	virtual void OnSkillTriggered() override;
};
