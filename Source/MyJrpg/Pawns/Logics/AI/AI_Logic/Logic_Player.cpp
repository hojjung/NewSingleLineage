#include "Logic_Player.h"
#include "NavigationSystem.h"
#include "MyJrpg/Actors/Field/ItemActor.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"
#include "Navigation/PathFollowingComponent.h"

void ULogic_Player::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);
	
	check(m_Owner);

	m_Pl = Cast<AMyPlayerPawn>(m_Owner);

	m_CurrentState = EFSM::Idle;
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
	case EFSM::Interacting: return TEXT("Interacting");
		break;
	}

	return TEXT("None");
}

void ULogic_Player::CheckSetState()
{
	IFocusable* Focused = m_Pl->GetFocusedTarget<>();
	
	if(m_Pl->IsManualMoving() || !Focused)
	{
		m_CurrentState = EFSM::Idle;
		return;
	}

	ACombatUnitPawn* NPCPawn = Cast<ACombatUnitPawn>(Focused);

	if(!NPCPawn && (Focused || m_Pl->GetInteracting()))
	{
		m_CurrentState = EFSM::Interacting;

		return;
	}
	
	m_Pl->CancelInteract();

	if (!NPCPawn->IsAlive())
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
	if(m_Pl->GetInteracting())
	{
		return;
	}
	IFocusable* ItemTarget = m_Pl->GetFocusedTarget<>();
	
	ItemTarget->OnInteract();
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
