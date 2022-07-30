#include "Sensor_Player.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

USensor_Player::USensor_Player()
{
	m_SensingInterval = 0.3f;
}

void USensor_Player::Init(ACombatUnitPawn* owner)
{
	Super::Init(owner);
	
	m_PlayerOwner = Cast<AMyPlayerPawn>(owner);
	m_AryIgnores.Reset();
	m_AryIgnores.Add(m_PlayerOwner);
	m_Capsule = Cast<UCapsuleComponent>( m_PlayerOwner->GetComponentByClass(UCapsuleComponent::StaticClass()));
	m_AryInteractables.Reserve(10);
}

void USensor_Player::UpdateAISensing()
{
	FVector Loc = m_PlayerOwner->GetActorLocation();
	
	IFocusable* Target = nullptr;

	if(!m_PlayerOwner->IsUseFsm())
	{
		Target = GetNearTargetManual(900, m_PlayerOwner->GetAttackRange());	
	}
	else
	{
		
	}
	if(Target)
	{
		m_PlayerOwner->SetFocusedTarget(Target);
		return ;
	}
	m_PlayerOwner->SetFocusedTarget(nullptr);
}

IFocusable* USensor_Player::GetNearTargetManual(float SearchRange, float AttackRange)
{
	float MaxRange = MAX_flt;
	
	FVector Loc = m_PlayerOwner->GetActorLocation();
	
	m_AryInteractables.Reset(10);

	UMyGameInstance::Get->m_ZoneInst->GetNearNpcs<IFocusable>(m_PlayerOwner, m_AryInteractables, SearchRange);

	IFocusable* Target = nullptr;
	
	for (IFocusable* Focus : m_AryInteractables)
	{
		if (!Focus->IsInteractable())
		{
			continue;
		}

		AActor* FocusActor = Cast<AActor>(Focus);
		
		float NavLen = 0.f;

		FVector DestLoc = FocusActor->GetActorLocation();

		float Cost = 0.f;
		
		ENavigationQueryResult::Type ResultT = UMyLib::GetNavSys()->GetPathLengthAndCost(Loc, DestLoc, NavLen, Cost);

		ACombatUnitPawn* Mob = Cast<ACombatUnitPawn>(Focus);
		
		if (!Mob || !(Mob->IsAlive()))
		{
			NavLen += AttackRange;
		}

		if (NavLen > MaxRange)
		{
			continue;
		}

		MaxRange = NavLen;

		Target = Focus;
	}
	return Target;
}

