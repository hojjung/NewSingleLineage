#include "Skill_ChainLight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_ChainLight::USkill_ChainLight()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Found(
		TEXT("ParticleSystem'/Game/03_VisualEffect/BeamsAndLazers/Particles/Electric_P.Electric_P'"));
	m_Particle = Found.Object;
}

void USkill_ChainLight::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	ACombatUnitPawn* HitPawn = ExecuteChainLight(m_Player->GetActorLocation(),0);

	int Iter = 0;

	while (HitPawn && Iter < 3 + m_nSkillLevel)
	{
		HitPawn = ExecuteChainLight(HitPawn->GetActorLocation(), m_SkillData->m_fRange);

		Iter++;
	}
}

void USkill_ChainLight::EndSkillAnim()
{
	Super::EndSkillAnim();
	m_Hitten.Reset();
	for(UParticleSystemComponent* Effects : m_ArySpawnedLaser)
	{
		Effects->DestroyComponent();
		Effects = nullptr;
	}
	m_ArySpawnedLaser.Reset();
}

ACombatUnitPawn* USkill_ChainLight::ExecuteChainLight(FVector&& start, float range)
{
	ACombatUnitPawn* Pawn = UMyGameInstance::Get->m_ZoneInst->GetNearNpc(start, range, &m_Hitten);

	if (!Pawn)
	{
		return nullptr;
	}
	
	UParticleSystemComponent* SpawnedLaser = UMyGameInstance::Get->m_EffectManager->SpawnEffect(Pawn,true,m_Particle);
	SpawnedLaser->SetBeamSourcePoint(0,start,0);
	SpawnedLaser->SetBeamEndPoint(0, Pawn->GetActorLocation());
	m_ArySpawnedLaser.Add(SpawnedLaser);
	
	Pawn->TakeDmg(GetDamage(),m_Player);
	
	m_Hitten.Add(Pawn);

	return Pawn;
}
