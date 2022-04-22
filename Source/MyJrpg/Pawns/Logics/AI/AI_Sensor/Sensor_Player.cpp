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
	if (m_PlayerOwner->IsInputMoving())
	{
		m_PlayerOwner->SetFocusedTarget(nullptr);
		
		return;
	}
	ACombatUnitPawn* TargetPawn =m_PlayerOwner->GetFocusedTarget<ACombatUnitPawn>();
	
	if(TargetPawn)
	{
		if(TargetPawn->IsAlive())
		{
			return;
		}
	}
	
	TargetPawn = GetSensedPawn();

	if(TargetPawn)
	{
		m_PlayerOwner->SetFocusedTarget(TargetPawn);

		return;
	}

	m_PlayerOwner->SetFocusedTarget(nullptr);
}

ACombatUnitPawn* USensor_Player::GetSensedPawn()
{
	return UMyGameInstance::Get->m_SpawnManager->GetNearNpc(m_PlayerOwner->GetActorLocation());
}

