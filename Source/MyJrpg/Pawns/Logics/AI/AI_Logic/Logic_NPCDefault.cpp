#include "Logic_NPCDefault.h"
#include "NavigationSystem.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "Navigation/PathFollowingComponent.h"


void ULogic_NPCDefault::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_fIdleTimer = -1.f;

	m_fChaseFindTimer = -1.f;

	m_CurrentState = EFSM::Idle;

	m_fAttackRange = pawnUnit->GetAttackRange();

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &ULogic_NPCDefault::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &ULogic_NPCDefault::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &ULogic_NPCDefault::OnCombat;
	//
	ResetStartPosition(m_Owner->GetActorLocation());
}

void ULogic_NPCDefault::Tick(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	FVector Loc = m_Owner->GetActorLocation();

	FString TT = CurrentState();

	PRINT3D(Loc, TT);
}

void ULogic_NPCDefault::SetIdle()
{
	if (m_CurrentState == EFSM::Idle)
	{
		return;
	}
	m_CurrentState = EFSM::Idle;

	m_Owner->SetFocusedTarget(nullptr);
		
	m_Owner->StopMove();
}

void ULogic_NPCDefault::CheckSetState()
{
	if (!m_Owner->GetFocusedTarget())
	{
		SetIdle();
	}
	else
	{
		ACombatUnitPawn* TargetNPC =  Cast<ACombatUnitPawn>(m_Owner->GetFocusedTarget());

		if (!TargetNPC->IsAlive())
		{
			SetIdle();

			return;
		}

		float Range = GetAttackRangeSqr();

		if (m_Owner->IsRangeMode() && !m_Owner->LineOfSightTo(TargetNPC))
		{
			Range = 100;
		}
		
		if (!CheckTargetRange(Range))
		{
			m_CurrentState = EFSM::Chase;
			return;
		}	

		m_CurrentState = EFSM::Combat;
	}
}

FString ULogic_NPCDefault::CurrentState()
{
	switch (m_CurrentState)
	{
	case Idle: return TEXT("Idle");
		break;
	case Chase: return TEXT("Chase");
		break;
	case Combat: return TEXT("Combat");
		break;
	}

	return TEXT("None");
}

void ULogic_NPCDefault::OnIdle()
{
	EPathFollowingStatus::Type Status = m_Owner->GetPfComp()->GetStatus();

	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_fDeltaTime;

		return;
	}

	FNavLocation Result;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (EPathFollowingStatus::Idle == Status)
	{
		if (!NavSys->GetRandomPointInNavigableRadius(m_StartPoint, 400.f, Result))
		{
			return;
		}

		m_Owner->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	}
}

void ULogic_NPCDefault::OnChase()
{
	EPathFollowingRequestResult::Type Result = EPathFollowingRequestResult::Failed;

	Result = m_Owner->MoveToActor(m_Owner->GetFocusedTarget());
}

void ULogic_NPCDefault::OnCombat()
{
	m_Owner->HomingRotateToTarget();

	if(CheckAngle(60))
	{
		m_Owner->TryAttack();
	}
}

void ULogic_NPCDefault::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
