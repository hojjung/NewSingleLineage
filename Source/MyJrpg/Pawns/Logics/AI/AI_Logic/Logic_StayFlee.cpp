// Fill out your copyright notice in the Description page of Project Settings.


#include "Logic_StayFlee.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"


void ULogic_StayFlee::Init(ACombatUnitPawn* pawnUnit)
{
	Super::Init(pawnUnit);

	m_fIdleTimer = -1.f;

	m_fFleeTimer = -1.f;

	m_CurrentState = EFSM::Idle;

	m_fAttackRange = pawnUnit->GetAttackRange();

	m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
	//
	m_AryStateFunction[static_cast<int>(EFSM::Idle)] = &ULogic_StayFlee::OnIdle;

	ResetStartPosition(m_Owner->GetActorLocation());
}

void ULogic_StayFlee::Tick(float deltaTime)
{
	m_fDeltaTime = deltaTime;

	CheckSetState();

	(this->*m_AryStateFunction[static_cast<int>(m_CurrentState)])();

	PRINT3DF(m_Owner->GetActorLocation(),"%s",*CurrentState());
}

void ULogic_StayFlee::SetIdle()
{
	if (m_CurrentState == EFSM::Idle)
	{
		return;
	}
	m_CurrentState = EFSM::Idle;

	m_Owner->SetFocusedTarget(nullptr);

	m_fFleeTimer  = 0.f;
		
	//m_Owner->StopMove();
}

void ULogic_StayFlee::CheckSetState()
{
	ACombatUnitPawn* TargetNPC =  Cast<ACombatUnitPawn>(m_Owner->GetFocusedTarget());
	
	if (!TargetNPC || !TargetNPC->IsAlive())
	{
		SetIdle();
	}
	else
	{
		OnTargetFocused(m_Owner);
	}
}

FString ULogic_StayFlee::CurrentState()
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

void ULogic_StayFlee::OnIdle()
{
	
}

void ULogic_StayFlee::OnFocusHate()
{
	if (m_fFleeTimer > 0.f)
	{
		m_fFleeTimer -= m_fDeltaTime;

		return;
	}
	FVector RunDir = (m_Owner->GetActorLocation() - m_Owner->GetFocusedTarget<AActor>()->GetActorLocation()).GetSafeNormal2D();

	float RandDistLen = FMath::RandRange(m_Owner->GetSightRange(),m_Owner->GetSightRange() * 2.f);

	float RandRad = FMath::RandRange(m_Owner->GetSightRange(),m_Owner->GetSightRange() * 2.f);

	RunDir *= RandDistLen;

	FNavLocation Result;

	if (!UMyLib::GetNavSys()->GetRandomPointInNavigableRadius(m_StartPoint + RunDir, RandRad, Result))
	{
		return;
	}
	
	m_fFleeTimer = FMath::FRandRange(1.f, 2.f);
	
	m_Owner->MoveToLocation(Result);
}

void ULogic_StayFlee::ResetStartPosition(FVector loc)
{
	m_StartPoint = loc;
}
