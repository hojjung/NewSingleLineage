// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_BuffRangeUp.h"

#include "MyJrpg/DataTables/SkillData.h"

void USkill_BuffRangeUp::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	//m_Player->AddAtkRange(m_SkillData->m_fRange);
}

void USkill_BuffRangeUp::EndDuration()
{
	Super::EndDuration();
	//m_Player->SubAtkRange(m_SkillData->m_fRange);
}
