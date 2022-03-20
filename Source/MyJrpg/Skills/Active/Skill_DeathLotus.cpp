// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_DeathLotus.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_DeathLotus::USkill_DeathLotus(const FObjectInitializer& obj):Super(obj)
{
	
}

void USkill_DeathLotus::OnSkillTriggered()
{
	Super::OnSkillTriggered();
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits, 0, m_SkillData->m_fRadius))
	{
		return;
	}
	FVector Loc = m_Player->GetActorLocation();
	
	for(AActor* Mob : Hits)
	{
		AMonsterPawn* Monster =  Cast<AMonsterPawn>(Mob);
		Shoot(Monster, Loc);
	}
}

void USkill_DeathLotus::Shoot(AMonsterPawn* mob, const FVector& start)
{
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABullet* Bullet= UMyLib::GetUWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), start, FRotator::ZeroRotator, Param);
	Bullet->Init(GetDamage(),m_Player,m_Particle,true);
	Bullet->StartBullet(start,mob);
}

