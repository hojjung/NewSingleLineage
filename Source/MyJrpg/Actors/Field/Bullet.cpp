#include "Bullet.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "Particles/ParticleSystemComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	m_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect01"));
	m_Effect->SetupAttachment(RootComponent);
	m_Effect->SetAutoActivate(false);
	m_Effect->SetActive(false);

	m_Move = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Move"));
	m_Move->SetUpdatedComponent(GetRootComponent());
	
	m_Move->bIsHomingProjectile = true;
	m_Move->HomingAccelerationMagnitude = 6000;
	m_Move->bRotationFollowsVelocity = true;
	m_Move->bRotationRemainsVertical = true;
	m_Move->MaxSpeed = 1000;
	m_Move->ProjectileGravityScale = 0;
	m_Move->bShouldBounce = false;
	
	m_Move->bSweepCollision = false;

	m_bDestoryOnEnd = true;
}

void ABullet::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	if(!m_TargetUnit.Get())
	{
		EndBullet();
	}

	if(IsEqual())
	{
		OnChaseDone();
	}
}

bool ABullet::IsEqual()
{
	return FVector::DistSquared2D(GetActorLocation(),m_TargetUnit->GetActorLocation()) <= 10000;
}

void ABullet::OnChaseDone()
{
	if (m_TargetUnit->IsAlive())
	{
		m_TargetUnit->TakeDmg(m_fDmg,m_OwnerUnit.Get());//플레이어랑 몬스터랑 다른데
	}

	EndBullet();
}

void ABullet::Init(float dmg,ACombatUnitPawn* ownerUnit, UParticleSystem* effect, bool destoryOnEnd, FVector scale)
{
	m_OwnerUnit = ownerUnit;
	m_Effect->SetTemplate(effect);
	m_bDestoryOnEnd = destoryOnEnd;
	m_Effect->SetRelativeScale3D(scale);
	m_fDmg = dmg;
}

void ABullet::StartBullet(FVector start, ACombatUnitPawn* targetUnit)
{
	SetActorLocation(start);
	
	SetActive(true);
	
	FRotator Rot = m_OwnerUnit->GetActorRotation();
	
	SetActorRotation(Rot);

	m_TargetUnit = targetUnit;

	m_Move->HomingTargetComponent = m_TargetUnit->GetBulletTarget();
}

void ABullet::EndBullet()
{
	SetActive(false);

	m_Move->Velocity = FVector::ZeroVector;

	if(m_bDestoryOnEnd)
	{
		Destroy();	
	}
}

void ABullet::SetActive(bool isActive)
{
	SetHidden(!isActive);

	SetActorTickEnabled(isActive);

	m_Effect->SetVisibility(isActive);
	
	if (!isActive)
	{
		m_Effect->Deactivate();
	}
	
	for(auto Comp : GetComponents())
	{
		Comp->Activate(isActive);
		Comp->SetComponentTickEnabled(isActive);
	}
}