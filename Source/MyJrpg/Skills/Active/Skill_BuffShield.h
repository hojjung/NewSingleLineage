#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Skills/Skill_BuffBase.h"
#include "Skill_BuffShield.generated.h"

UCLASS()
class MYJRPG_API USkill_BuffShield : public USkill_BuffBase
{
	GENERATED_BODY()

protected:
	virtual void OnSkillTriggered() override;

	virtual void EndDuration() override;
};
