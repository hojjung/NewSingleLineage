// Fill out your copyright notice in the Description page of Project Settings.


#include "Logic_Flee.h"

#include "MyJrpg/MyLib.h"


void ULogic_Flee::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_fIdleTimer = -1.f;

	m_fFleeTimer = -1.f;

	m_CurrentState = EFSM::Idle;

	m_fAttackRange = pawnUnit->GetAttackRange();

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &ULogic_Flee::OnIdle;

	m_AryStateFunction[static_cast<int>(EFSM::Flee)] = &ULogic_Flee::OnFlee;

	ResetStartPosition(m_Owner->GetActorLocation());
}

void ULogic_Flee::Tick(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	PRINT3DF(m_Owner->GetActorLocation(),"%s",*CurrentState());
}

void ULogic_Flee::SetIdle()
{
	if (m_CurrentState == EFSM::Idle)
	{
		return;
	}
	m_CurrentState = EFSM::Idle;

	m_Owner->SetFocusedTarget(nullptr);
		
	//m_Owner->StopMove();
}

void ULogic_Flee::CheckSetState()
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

		if ( m_CurrentState == EFSM::Flee && !CheckTargetRange(m_fAttackRangeSqr))
		{
			SetIdle();
		}
		else
		{
			m_CurrentState = EFSM::Flee;
		}
	}
}

FString ULogic_Flee::CurrentState()
{
	switch (m_CurrentState)
	{
	case Idle: return TEXT("Idle");
		break;
	case Flee: return TEXT("Flee");
		break;
	}
	return TEXT("None");
}

void ULogic_Flee::OnIdle()
{
	EPathFollowingStatus::Type Status = m_Owner->GetPfComp()->GetStatus();

	if (m_fIdleTimer > 0.f)
	{
		m_fIdleTimer -= m_fDeltaTime;

		return;
	}

	int RandIndex = FMath::RandRange(0,4);

	if(RandIndex == 0)
	{
		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
		return;
	}

	FNavLocation Result;

	if (EPathFollowingStatus::Idle == Status)
	{
		if (!UMyLib::GetNavSys()->GetRandomPointInNavigableRadius(m_StartPoint, 1400.f, Result))
		{
			return;
		}

		m_Owner->MoveToLocation(Result);

		m_fIdleTimer = FMath::FRandRange(3.f, 7.f);
	}
}

void ULogic_Flee::OnFlee()
{
	if (m_fFleeTimer > 0.f)
	{
		m_fFleeTimer -= m_fDeltaTime;

		return;
	}
	FVector RunDir = (m_Owner->GetActorLocation() - m_Owner->GetFocusedTarget<AActor>()->GetActorLocation()).GetSafeNormal2D();

	float RandDistLen = FMath::RandRange(750,1500);

	float RandRad = FMath::RandRange(800,1200);

	RunDir *= RandDistLen;

	FNavLocation Result;

	if (!UMyLib::GetNavSys()->GetRandomPointInNavigableRadius(m_Owner->GetActorLocation() + RunDir, RandRad, Result))
	{
		return;
	}
	
	m_fFleeTimer = FMath::FRandRange(1.f, 2.f);
	
	m_Owner->MoveToLocation(Result);
}

void ULogic_Flee::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
