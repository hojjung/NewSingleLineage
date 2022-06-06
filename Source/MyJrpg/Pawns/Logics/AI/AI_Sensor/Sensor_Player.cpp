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
	
	IFocusable* Target = UMyGameInstance::Get->m_ZoneInst->GetNearTarget(Loc, 0, m_PlayerOwner->IsUseFsm() ? AStructureActor::StaticClass() : nullptr );
	
	if(Target)
	{
		m_PlayerOwner->SetFocusedTarget(Target);
		return ;
	}
	
	m_PlayerOwner->SetFocusedTarget(nullptr);
}