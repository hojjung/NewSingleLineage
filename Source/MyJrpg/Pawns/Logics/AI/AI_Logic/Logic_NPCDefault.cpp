#include "Logic_NPCDefault.h"
#include "NavigationSystem.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "Navigation/PathFollowingComponent.h"


void ULogic_NPCDefault::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_fAlertTimer = -1.f;

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
	if(m_fAlertTimer > 0.f)
	{
		m_fAlertTimer -= m_fDeltaTime;

		return;
	}
	m_fAlertTimer = FMath::FRandRange(2.f, 5.f);
	
	FVector Start = m_Owner->GetActorLocation();

	TArray<AActor*> OutHits;
	
	//DrawDebugSphere(GetWorld(),Start,450,12,FColor::Red,false,1);
	if(!UMyLib::SphereOverlapActors(m_Owner,m_Owner->GetActorRotation(),Start,450,
		m_Owner->GetTraceObjTypes(),AMonsterPawn::StaticClass(),m_Owner->GetTraceIgnoredActors(),OutHits))
	{
		return;
	}

	for(auto Ally : OutHits)
	{
		AMonsterPawn* Mob = Cast<AMonsterPawn>(Ally);

		//if(Mob->GetTeamID() == m_Owner->GetTeamID())
		{
			Mob->SetFocusedTarget(m_Owner->GetFocusedTarget<IFocusable>());
		}
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
