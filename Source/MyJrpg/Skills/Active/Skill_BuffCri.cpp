// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_BuffCri.h"

#include "MyJrpg/DataTables/SkillData.h"

void USkill_BuffCri::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	GetPlayerStatus()->AddPlusCriPer(m_SkillData->m_fDamage);
}

void USkill_BuffCri::EndSkillAnim()
{
	Super::EndSkillAnim();
	GetPlayerStatus()->SubPlusCriPer(m_SkillData->m_fDamage);
}
