#include "Logic_Stay.h"

#include "NavigationSystem.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void ULogic_Stay::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_fAlertTimer = -1.f;

	m_fIdleTimer = -1.f;

	m_fChaseFindTimer = -1.f;

	m_CurrentState = EFSM::Idle;

	m_fAttackRange = pawnUnit->GetAttackRange();

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &ULogic_Stay::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Chase)] = &ULogic_Stay::OnChase;

	m_AryStateFunction[static_cast<int>(EFSM::Combat)] = &ULogic_Stay::OnCombat;
	//
	ResetStartPosition(m_Owner->GetActorLocation());
}

void ULogic_Stay::Tick(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	FVector Loc = m_Owner->GetActorLocation();

	FString TT = CurrentState();

	PRINT3D(Loc, TT);
}

void ULogic_Stay::SetIdle()
{
	if (m_CurrentState == EFSM::Idle)
	{
		return;
	}
	m_fIdleTimer = 0.f;
	
	m_CurrentState = EFSM::Idle;

	m_Owner->SetFocusedTarget(nullptr);
		
	m_Owner->StopMove();
}

void ULogic_Stay::CheckSetState()
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

FString ULogic_Stay::CurrentState()
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

void ULogic_Stay::OnIdle()
{
	if(m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_fDeltaTime;

		return;
	}
	m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	
	FVector Loc = m_Owner->GetActorLocation();

	float DistSqr = FVector::DistSquared2D(Loc, m_StartPoint);

	if(DistSqr >= 90000)
	{
		m_Owner->MoveToLocation(m_StartPoint);
	}
	//Stay
}

void ULogic_Stay::OnChase()
{
	m_Owner->ChaseTarget();
}

void ULogic_Stay::OnCombat()
{
	m_Owner->HomingRotateToTarget();

	if(CheckAngle(60))
	{
		m_Owner->TryAttack();
	}
}

void ULogic_Stay::AlertEnemyToAllies()
{
	if(m_fAlertTimer > 0.f)
	{
		m_fAlertTimer -= m_fDeltaTime;

		return;
	}
	m_fAlertTimer = FMath::FRandRange(3.f, 7.f);
	
	FVector Start = m_Owner->GetActorLocation();

	TArray<AActor*> OutHits;
	
	if(!UMyLib::SphereOverlapActors(m_Owner,m_Owner->GetActorRotation(),Start,700,
		m_Owner->GetTraceObjTypes(),AMonsterPawn::StaticClass(),m_Owner->GetTraceIgnoredActors(),OutHits))
	{
		return;
	}

	IFocusable* FocusedTarget = m_Owner->GetFocusedTarget<IFocusable>();
	
	for(auto Ally : OutHits)
	{
		AMonsterPawn* Mob = Cast<AMonsterPawn>(Ally);

		if(Mob->GetFocusedTarget<IFocusable>() != FocusedTarget)
		{
			Mob->SetFocusedTarget(FocusedTarget);
		}
	}
}

void ULogic_Stay::OnFocusFriendly()
{
	OnFocusNeutral();
}

void ULogic_Stay::OnFocusNeutral()
{
	SetIdle();
}

void ULogic_Stay::OnFocusHate()
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

void ULogic_Stay::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
