#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_Consume.generated.h"

/**
 * 가까이 있는 적 하나를
 * 해당 스킬로 몬스터 처치시 체력 회복
 */

UCLASS()
class MYJRPG_API USkill_Consume : public USkillBase
{
	GENERATED_BODY()

public:
	USkill_Consume();

protected:
	UPROPERTY()
	UParticleSystem* m_Particle;
	
protected:
	virtual void OnSkillTriggered() override;
	
};
