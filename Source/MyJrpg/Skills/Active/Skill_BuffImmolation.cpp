#include "Skill_BuffImmolation.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Managers/MyGameInstance.h"

USkill_BuffImmolation::USkill_BuffImmolation(const FObjectInitializer& obj):Super(obj)
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundSt(
			TEXT("ParticleSystem'/Game/03_VisualEffect/FX/Effects/FX_Mobile/P_Bomb.P_Bomb'"));
	m_Particle = FoundSt.Object;
}

void USkill_BuffImmolation::OnTick(float delta)
{
	Super::OnTick(delta);
	
	float Damage = m_SkillData->m_fDamage * delta;
	
	TArray<AActor*> Hits;
	
	if(!TraceSphere(Hits, m_SkillData->m_fRange, m_SkillData->m_fRadius))
	{
		return;
	}
	
	UMyGameInstance::Get->m_EffectManager->SpawnEffect(m_Player,true,m_Particle);
	
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		
		Monster->TakeDmg(Damage, m_Player);
	}
}
