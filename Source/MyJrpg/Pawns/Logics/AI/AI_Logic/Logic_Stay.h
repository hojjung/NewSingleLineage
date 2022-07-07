// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_LogicBase.h"
#include "Logic_Stay.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ULogic_Stay : public UAI_LogicBase
{
	GENERATED_BODY()

	
public:
	enum EFSM
	{
		Idle,
		Chase,
		Combat,
		Length
	};

protected:
	EFSM m_CurrentState;

	FVector m_StartPoint;

	float m_fIdleTimer;

	float m_fChaseFindTimer;

	float m_fAttackRange; //from startPos

	float m_fAttackRangeSqr; //from startPos

	typedef void (ULogic_Stay::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

	float m_fDeltaTime;

	float m_fAlertTimer;

protected:
	FString CurrentState();
	
	virtual void CheckSetState();
	
	void OnIdle();

	void OnChase();
	
	virtual void OnCombat();

	void AlertEnemyToAllies();

protected:
	virtual void OnFocusFriendly() override;

	virtual void OnFocusNeutral() override;

	virtual void OnFocusHate() override;

	

public:
	virtual void Init(ACombatUnitPawn* pawnUnit) override;
	
	virtual void Tick(float delta) override;
	void SetIdle();

	void ResetStartPosition(FVector loc);
	
	FORCEINLINE float GetAttackRange()
	{
		return m_fAttackRange;
	}

	FORCEINLINE float GetAttackRangeSqr()
	{
		return m_fAttackRangeSqr;
	}
};
