// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_BuffBase.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"



void USkill_BuffBase::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	UMyGameInstance::Get->m_PlayerStatManager->AddBuff(this);
}

bool USkill_BuffBase::CanUseSkill()
{
	if(!m_Player)
	{
		m_Player =  UMyLib::GetPlayer();	
	}
	return CheckCooltime();
}

float USkill_BuffBase::GetDuration() const
{
	return m_SkillData->m_fDuration;
}

void USkill_BuffBase::OnTick(float delta)//usually delta is 0.1f
{
	
}

void USkill_BuffBase::EndDuration()
{
	
}