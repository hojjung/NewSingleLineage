#include "Logic_NPCDefault.h"
#include "NavigationSystem.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "Navigation/PathFollowingComponent.h"


void ULogic_NPCDefault::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_bAlertOnce = false;

	m_NearMobs.Reserve(10);

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
		OnTargetFocused(m_Owner);
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
	m_Owner->ChaseTarget();
}

void ULogic_NPCDefault::OnCombat()
{
	m_Owner->HomingRotateToTarget();

	if(CheckAngle(60))
	{
		m_Owner->TryAttack();
	}
}

void ULogic_NPCDefault::AlertEnemyToAllies()
{
	if(m_bAlertOnce)
	{
		return;
	}
	m_bAlertOnce = true;
	
	UMyGameInstance::Get->m_ZoneInst->GetNearNpcs(m_Owner,m_NearMobs,500);

	IFocusable* FocusedTarget = m_Owner->GetFocusedTarget<IFocusable>();
	
	for(ACombatUnitPawn* Ally : m_NearMobs)
	{
		Ally->SetFocusedTarget(FocusedTarget);
	}
}

void ULogic_NPCDefault::OnFocusFriendly()
{
	OnFocusNeutral();
}

void ULogic_NPCDefault::OnFocusNeutral()
{
	SetIdle();
}

void ULogic_NPCDefault::OnFocusHate()
{
	ACombatUnitPawn* TargetNPC =  Cast<ACombatUnitPawn>(m_Owner->GetFocusedTarget());
	
	if (!TargetNPC->IsAlive())
	{
		SetIdle();

		return;
	}
	AlertEnemyToAllies();

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

void ULogic_NPCDefault::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
