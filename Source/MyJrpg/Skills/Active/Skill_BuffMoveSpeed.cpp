#include "Skill_BuffMoveSpeed.h"

#include "MyJrpg/MyLib.h"

void USkill_BuffMoveSpeed::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	GetPlayerStatus()->AddMultiMoveSpeed(m_SkillData->m_fDamage);
}

void USkill_BuffMoveSpeed::EndDuration()
{
	Super::EndDuration();
	GetPlayerStatus()->SubMultiMoveSpeed(m_SkillData->m_fDamage);
}

