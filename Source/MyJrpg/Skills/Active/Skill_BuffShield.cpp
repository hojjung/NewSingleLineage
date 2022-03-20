#include "Skill_BuffShield.h"
#include "MyJrpg/DataTables/SkillData.h"

void USkill_BuffShield::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	GetPlayerStatus()->AddPlusDmgReduc(m_SkillData->m_fDamage);
}

void USkill_BuffShield::EndDuration()
{
	Super::EndDuration();
	GetPlayerStatus()->SubPlusDmgReduc(m_SkillData->m_fDamage);
}
