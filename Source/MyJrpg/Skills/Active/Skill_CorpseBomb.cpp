#include "Skill_CorpseBomb.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"


USkill_CorpseBomb::USkill_CorpseBomb(const FObjectInitializer& obj):Super(obj)
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundSt(
				TEXT("ParticleSystem'/Game/03_VisualEffect/FX/Effects/FX_Mobile/P_Bomb.P_Bomb'"));
	m_Particle = FoundSt.Object;
}

void USkill_CorpseBomb::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	
	ACombatUnitPawn* Pawn =  UMyGameInstance::Get->m_ZoneInst->GetNearNpc(m_Player->GetActorLocation(), m_SkillData->m_fRange);
	
	if (!Pawn)
	{
		return;
	}
	
	ExecuteCorpseBomb(Pawn);
}

void USkill_CorpseBomb::ExecuteCorpseBomb(ACombatUnitPawn* pawn)
{
	if(!pawn->IsAlive())
	{
		return ;
	}
	
	pawn->TakeDmg(m_SkillData->m_fDamage,m_Player);

	if(pawn->IsAlive())
	{
		return ;
	}
	FVector Start = pawn->GetActorLocation();
	
	UMyGameInstance::Get->m_EffectManager->SpawnEffect(Start,m_Particle);
	
	TArray<AActor*> Hits;
	
	if(!TraceSphere(Hits, Start, m_SkillData->m_fRadius))
	{
		return;
	}
	
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);

		ExecuteCorpseBomb(Monster);
	}
}
