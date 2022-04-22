#include "Skill_SilenceCut.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "MyJrpg/Pawns/Components/MyMovement.h"

void USkill_SilenceCut::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	ACombatUnitPawn* TargetMob = m_Player->GetFocusedTarget<ACombatUnitPawn>();

	if(!TargetMob)
	{
		return;
	}
	
	FVector Dir = TargetMob->GetActorLocation() - m_Player->GetActorLocation();

	Dir = Dir.GetSafeNormal();

	m_Player->GetMove()->SetImpact(Dir * m_SkillData->m_fRange);
	
	TargetMob->TakeDmg(m_SkillData->m_fDamage, m_Player);
	
	TargetMob->TakeSilence(m_SkillData->m_fDuration);
}
