// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Sensor_LogicBase.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "Sensor_Player.generated.h"

class AMyPlayerPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API USensor_Player : public USensor_LogicBase
{
	GENERATED_BODY()

public:
	USensor_Player();
protected:
	UPROPERTY()
	AMyPlayerPawn* m_PlayerOwner;
	UPROPERTY()
	UCapsuleComponent* m_Capsule;
	UPROPERTY()
	TArray<AActor*> m_AryIgnores;

	TArray<IFocusable*> m_AryInteractables;
public:
	virtual void Init(ACombatUnitPawn* owner) override;
	
protected:
	virtual void UpdateAISensing() override;
	
	IFocusable* GetNearTargetManual(float SearchRange, float AttackRange);
	//오토는, 우호적인 사람, 시체, 건물을 무시한다
	IFocusable* GetNearTargetAuto(float AttackRange);
};


