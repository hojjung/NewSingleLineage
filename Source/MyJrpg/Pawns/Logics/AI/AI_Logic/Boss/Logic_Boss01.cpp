#include "Logic_Boss01.h"
#include "NavigationSystem.h"
#include "Engine/DecalActor.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

ULogic_Boss01::ULogic_Boss01()
{
	
}

void ULogic_Boss01::Init(ACombatUnitPawn* owner)
{
	Super::Init(owner);

	m_Skill01 = NewObject<UMobSkill_CircleSelfAOE>(this);
	m_Skill01->Init(m_Owner);
}

void ULogic_Boss01::OnCombat()
{
	if(m_Skill01->IsUsing())
	{
		return;
	}
	
	m_fSkillTimer += m_fDeltaTime;
	
	if(m_fSkillTimer < m_fSkillUseCD)
	{
		Super::OnCombat();
		
		return;
	}
	m_fSkillTimer = 0;

	m_Skill01->UseSkill();
}

void ULogic_Boss01::CheckSetState()
{
	if(m_Skill01->IsUsing())
	{
		m_CurrentState = EFSM::Combat;
		return;
	}
	if (!m_Owner->GetFocusedTarget())
	{
		SetIdle();
	}
	else
	{
		ACombatUnitPawn* TargetNPC =  Cast<ACombatUnitPawn>(m_Owner->GetFocusedTarget());

		if (!TargetNPC->IsAlive())
		{
			SetIdle();

			return;
		}

		float Range = GetAttackRangeSqr();

		if (m_Owner->IsRangeMode() && !m_Owner->LineOfSightTo(TargetNPC))
		{
			Range = 100;
		}
		
		if (!CheckTargetRange(Range))
		{
			m_CurrentState = EFSM::Chase;
			return;
		}	

		m_CurrentState = EFSM::Combat;
	}
}
