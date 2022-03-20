#include "Skill_Harvest.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_Harvest::USkill_Harvest()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Found(
		TEXT("ParticleSystem'/Game/04_VisualEffect/FantasyRPGVFXPack/Particles/PS_Hit_Claws_Red.PS_Hit_Claws_Red'"));
	m_Particle = Found.Object;
}

void USkill_Harvest::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits, m_SkillData->m_fRange, m_SkillData->m_fRadius))
	{
		return;
	}
	int Iter = 0;
	
	for(AActor* Mob : Hits)
	{
		if (Iter >= 10)
		{
			break;
		}
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		
		Monster->TakeDmg(GetDamage(), m_Player);
		
		UMyGameInstance::Get->m_EffectManager->SpawnEffect(Monster,true,m_Particle);
		
		float HealHp = m_Player->GetStat().m_MaxHp * 0.05f;
		
		m_Player->TakeHeal(HealHp);

		Iter++;
	}
}
