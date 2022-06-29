#include "PlSkillAuto.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UPlSkillAuto::Init()
{
	m_bAutoUse = false;
	
	m_Pl = UMyLib::GetPlayer();
	
	m_PtrSkills = &UMyGameInstance::Get->m_SkillManager->GetEquipSkillInstAry();
}

void UPlSkillAuto::TryAddWaitQue(USkillBase* skillInst)
{
	if(m_SetSkillInstsWait.Contains(skillInst) || !skillInst->CanUseSkill())
	{
		return;
	}

	m_QueSkillInstsWait.Enqueue(skillInst);
	m_SetSkillInstsWait.Add(skillInst);
}

void UPlSkillAuto::TryUseWaitQue()
{
	USkillBase* ReadySkill;
	
	if(!m_QueSkillInstsWait.Dequeue(ReadySkill))
	{
		return;
	}

	m_SetSkillInstsWait.Remove(ReadySkill);

	UMyGameInstance::Get->m_SkillManager->ExecuteSkill(ReadySkill);
}

void UPlSkillAuto::Tick(float deltaTime)
{
	if (!m_bAutoUse ||!m_Pl->IsAlive())
	{
		return;
	}
	
	for(auto& Skill : *m_PtrSkills)
	{
		if(!Skill)
		{
			continue;
		}
		Skill->Tick(deltaTime);

		TryAddWaitQue(Skill);
	}

	if (m_Pl->GetSkillUsing())
	{
		return;
	}

	TryUseWaitQue();
}

void UPlSkillAuto::SetUseAuto(bool bUseAuto)
{
	m_bAutoUse = bUseAuto;

	m_QueSkillInstsWait.Empty();
	
	m_SetSkillInstsWait.Reset();
}
