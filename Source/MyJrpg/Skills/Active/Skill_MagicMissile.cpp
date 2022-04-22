#include "Skill_MagicMissile.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USkill_MagicMissile::USkill_MagicMissile()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
	FoundBullet(TEXT("ParticleSystem'/Game/04_VisualEffect/FantasyRPGVFXPack/Particles/PS_MIssle_ArcaneFireball.PS_MIssle_ArcaneFireball'"));
	
	m_BaseBullet = FoundBullet.Object;
}

void USkill_MagicMissile::SetSkill(FName id, const FSkillDataRow& dataRow, int skillLevel)
{
	Super::SetSkill(id, dataRow, skillLevel);
	m_Pool = NewObject<UBulletPool>(this);
	m_Pool->InitPool(GetDamage(),12,m_BaseBullet,m_Player,0.3f);
}

void USkill_MagicMissile::ShootBullet()
{
	FVector OffSet(FMath::RandRange(-100,100),FMath::RandRange(-100,100),FMath::RandRange(-100,100));
		
	m_Pool->ShootBullet(TEXT("Trail_End_L"),OffSet,m_Player->GetFocusedTarget<ACombatUnitPawn>());
}

void USkill_MagicMissile::OnSkillTriggered()
{
	int Iter = 0;

	while (Iter < m_nSkillLevel)
	{
		FTimerHandle Handle;
		
		GetWorld()->GetTimerManager().SetTimer(Handle,this,&USkill_MagicMissile::ShootBullet,1,false,(Iter) * 0.26f);
		
		Iter++;
	}
}
