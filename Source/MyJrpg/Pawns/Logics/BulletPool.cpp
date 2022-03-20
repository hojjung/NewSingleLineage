#include "BulletPool.h"
#include "MyJrpg/MyLib.h"

void UBulletPool::InitPool(float dmg, int count, UParticleSystem* particle, ACombatUnitPawn* owner,float scale)
{
	m_Owner = owner;
	
	m_Particle = particle;
	
	m_AryBullets.Reset();

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	int	iter = 0;

	while (iter < count)
	{
		ABullet* Bullet = UMyLib::GetUWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Param);

		Bullet->Init(dmg, m_Owner, m_Particle, false, FVector(scale));

		Bullet->SetActive(false);
		
		m_AryBullets.Add(Bullet);

		iter++;
	}
}

void UBulletPool::ShootBullet(FName socketStart, ACombatUnitPawn* target)
{
	ShootBullet(socketStart,FVector::ZeroVector,target);
}

void UBulletPool::ShootBullet(FVector start, ACombatUnitPawn* target)
{
	m_AryBullets[m_nPoolIndex]->StartBullet(start, target);
	
	m_nPoolIndex++;

	if(m_AryBullets.Num() <= m_nPoolIndex)
	{
		m_nPoolIndex = 0;
	}
}

void UBulletPool::ShootBullet(FName socketStart, FVector start, ACombatUnitPawn* target)
{
	if(!UMyLib::CheckAngle(m_Owner,target,60))
	{
		m_Owner->StopAnimMontage();
		return;
	}
	ShootBullet(m_Owner->GetSkMesh()->GetSocketLocation(socketStart) + start,target);
}

void UBulletPool::HideAll()
{
	for(ABullet* Bullet : m_AryBullets)
	{
		Bullet->SetActive(false);
	}
}

void UBulletPool::KillAll()
{
	for(ABullet* Bullet : m_AryBullets)
	{
		Bullet->SetActive(false);
		Bullet->Destroy();
	}
}
