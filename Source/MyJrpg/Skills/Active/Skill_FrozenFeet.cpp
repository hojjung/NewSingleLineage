// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_FrozenFeet.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_FrozenFeet::USkill_FrozenFeet()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Found(
	TEXT("ParticleSystem'/Game/04_VisualEffect/FantasyRPGVFXPack/Particles/PS_IceTrap.PS_IceTrap'"));
	m_Particle = Found.Object;	
}

bool USkill_FrozenFeet::CanUseSkill()
{
	if(!m_Player)
	{
		m_Player =  UMyLib::GetPlayer();	
	}
	return CheckCooltime();
}

void USkill_FrozenFeet::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits, 0, m_SkillData->m_fRadius))
	{
		return;
	}
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		
		Monster->TakeDmg(GetDamage(), m_Player);
		
		UMyGameInstance::Get->m_EffectManager->SpawnEffect(Monster->GetActorLocation(),m_Particle,2);

		Monster->TakeStun(m_SkillData->m_fDuration);
		
		//Frozen Need
	}
}
