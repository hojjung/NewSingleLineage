// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "AI_LogicBase.h"
#include "Logic_NPCDefault.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ULogic_NPCDefault : public UAI_LogicBase
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

	typedef void (ULogic_NPCDefault::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

	float m_fDeltaTime;

protected:
	FString CurrentState();
	
	void CheckSetState();
	
	void OnIdle();

	void OnChase();
	
	void OnCombat();

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
