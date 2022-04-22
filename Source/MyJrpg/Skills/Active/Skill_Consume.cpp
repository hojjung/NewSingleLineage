#include "Skill_Consume.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"


USkill_Consume::USkill_Consume()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Found(
		TEXT("ParticleSystem'/Game/04_VisualEffect/FantasyRPGVFXPack/Particles/PS_Hit_Claws_Red.PS_Hit_Claws_Red'"));
	m_Particle = Found.Object;
}

void USkill_Consume::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	if(!m_Player->GetFocusedTarget())
	{
		return;
	}
	
	UMyGameInstance::Get->m_EffectManager->SpawnEffect(m_Player->GetFocusedTarget<ACombatUnitPawn>(),false,m_Particle);
	
	m_Player->GetFocusedTarget<ACombatUnitPawn>()->TakeDmg(GetDamage(),m_Player);
	
	float HealHp = m_Player->GetStat().m_MaxHp * 0.15f;
		
	m_Player->TakeHeal(HealHp);
}
