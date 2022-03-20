#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Skills/SkillBase.h"
#include "Skill_Thunderclap.generated.h"

/**
 * 주변에 범위 데미지
 */
UCLASS()
class MYJRPG_API USkill_Thunderclap : public USkillBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnSkillTriggered() override;
};
