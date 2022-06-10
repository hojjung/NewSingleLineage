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
}

void USensor_Player::UpdateAISensing()
{
	FVector Loc = m_PlayerOwner->GetActorLocation();
	
	IFocusable* Target = nullptr;

	if(m_PlayerOwner->IsUseFsm())
	{
		Target = UMyGameInstance::Get->m_ZoneInst->GetNearTarget(Loc, 0, AStructureActor::StaticClass(), true);
	}
	else
	{
		Target = UMyGameInstance::Get->m_ZoneInst->GetNearTarget(Loc, 0, nullptr, false);
	}
	
	if(Target)
	{
		m_PlayerOwner->SetFocusedTarget(Target);
		
		return ;
	}
	
	m_PlayerOwner->SetFocusedTarget(nullptr);
}