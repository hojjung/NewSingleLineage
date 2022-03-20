#include "Skill_ShieldThrow.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/Bullet.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_ShieldThrow::USkill_ShieldThrow(const FObjectInitializer& obj)
{
	
}

void USkill_ShieldThrow::OnSkillTriggered()
{
	Super::OnSkillTriggered();

	FVector StartLoc = m_Player->GetActorLocation();

	ACombatUnitPawn* HitPawn = Shoot(StartLoc);

	while (HitPawn)
	{
		HitPawn = Shoot(HitPawn->GetActorLocation());
	}
}

ACombatUnitPawn* USkill_ShieldThrow::Shoot(const FVector& start)
{
	ACombatUnitPawn* Pawn = UMyGameInstance::Get->m_SpawnManager->GetNearNpc(start, m_SkillData->m_fRange, &m_Hitten);

	if (!Pawn)
	{
		return nullptr;
	}
	m_Hitten.Add(Pawn);
	
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABullet* Bullet= UMyLib::GetUWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), start, FRotator::ZeroRotator, Param);
	Bullet->Init(GetDamage(),m_Player,m_Particle,true);
	Bullet->StartBullet(start,Pawn);

	return Pawn;
}