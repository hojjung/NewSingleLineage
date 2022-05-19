#include "Logic_Player.h"
#include "NavigationSystem.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "Navigation/PathFollowingComponent.h"

void ULogic_Player::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);
	
	check(m_Owner);

	m_CurrentState = EFSM::Idle;

	AMyPlayerPawn* PlayerPawn = Cast<AMyPlayerPawn>(m_Owner);
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &ULogic_Player::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Interacting)] = &ULogic_Player::OnInteract;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &ULogic_Player::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &ULogic_Player::OnCombat;

	//
	ResetStartPosition(m_Owner->GetActorLocation());
}

void ULogic_Player::Tick(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	FVector Loc = m_Owner->GetActorLocation();

	FString TT = CurrentState();

	PRINT3DGreen(Loc, TT);
}

void ULogic_Player::ForceSetStateIdle()
{
	m_CurrentState = EFSM::Idle;
}

FString ULogic_Player::CurrentState()
{
	switch (m_CurrentState)
	{
	case EFSM::Idle: return TEXT("Idle");
		break;
	case EFSM::Chase: return TEXT("Chase");
		break;
	case EFSM::Combat: return TEXT("Combat");
		break;
	}

	return TEXT("None");
}

void ULogic_Player::CheckSetState()
{
	AMyPlayerPawn* PlayerPawn = Cast<AMyPlayerPawn>(m_Owner);
	
	ACombatUnitPawn* NPCPawn = PlayerPawn->GetFocusedTarget<ACombatUnitPawn>();

	if(PlayerPawn->IsManualMoving())
	{
		m_CurrentState = EFSM::Idle;
		return;
	}

	if(!NPCPawn || !NPCPawn->IsAlive())//포커싱된게 없다면/죽었다면/싫어하지 않는다면
	{
		m_CurrentState = EFSM::Idle;
		return;
	}

	OnTargetFocused(NPCPawn);
}

void ULogic_Player::OnIdle()
{
	
}

void ULogic_Player::OnChase()
{
	m_Owner->ChaseTarget();
}

void ULogic_Player::OnCombat()
{
	m_Owner->HomingRotateToTarget();

	if(CheckAngle(60))
	{
		m_Owner->TryAttack();
	}
}

void ULogic_Player::OnInteract()
{
	
}

void ULogic_Player::OnFocusFriendly()
{
	m_CurrentState = EFSM::Idle;
}

void ULogic_Player::OnFocusNeutral()
{
	OnFocusFriendly();
}

void ULogic_Player::OnFocusHate()
{
	ACombatUnitPawn* NPCPawn = Cast<ACombatUnitPawn>( m_Owner->GetFocusedTarget());
	
	float Range = m_Owner->GetAttackRangeSqr();

	if (m_Owner->IsRangeMode())
	{
		if(!m_Owner->LineOfSightTo(NPCPawn))
		{
			Range = 100;	
		}
	}

	if (!CheckTargetRange(Range))
	{
		m_CurrentState = EFSM::Chase;
		return;
	}	

	m_CurrentState = EFSM::Combat;
}



void ULogic_Player::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
