#pragma once

#include "CoreMinimal.h"
#include "AI_LogicBase.h"
#include "Logic_Player.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API ULogic_Player : public UAI_LogicBase
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnManualMoveDone);

	enum EFSM
	{
		Idle,
		ManualMove,
		Chase,
		Combat,
		Interacting,
		Length
	};

protected:
	EFSM m_CurrentState;

	FVector m_StartPoint;

	float m_fDeltaTime;

	float m_fMeleeRange; //from startPos

	float m_fMeleeRangeSqr; //from startPos

	float m_fRangeRange; //from startPos

	float m_fRangeRangeSqr; //from startPos

	typedef void (ULogic_Player::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

protected:
	void OnIdle();

	void OnChase();
	
	void OnCombat();

	void OnInteract();

public:
	virtual void Init(ACombatUnitPawn* owner) override;

	void ResetStartPosition(FVector loc);

	virtual void Tick(float delta) override;

	void ForceSetStateIdle();

	FString CurrentState();

	void CheckSetState();
};
