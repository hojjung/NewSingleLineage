#pragma once

#include "CoreMinimal.h"
#include "AI_LogicBase.h"
#include "Logic_Player.generated.h"

class AMyPlayerPawn;
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

	typedef void (ULogic_Player::*FPtrState)(void);

	FPtrState m_AryStateFunction[static_cast<int>(EFSM::Length)];

	TWeakObjectPtr<AMyPlayerPawn> m_Pl;

protected:
	void OnIdle();

	void OnChase();
	
	void OnCombat();

	void OnInteract();

protected:
	virtual void OnFocusFriendly() override;

	virtual void OnFocusHate() override;

	virtual void OnFocusNeutral() override;

public:
	virtual void Init(ACombatUnitPawn* owner) override;

	void ResetStartPosition(FVector loc);

	virtual void Tick(float delta) override;

	void ForceSetStateIdle();

	FString CurrentState();

	void CheckSetState();
};
