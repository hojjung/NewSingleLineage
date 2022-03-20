// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "Sensor_LogicBase.h"
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

protected:
	UPROPERTY()
	AMyPlayerPawn* m_PlayerOwner;
	UPROPERTY()
	UCapsuleComponent* m_Capsule;
	UPROPERTY()
	TArray<AActor*> m_AryIgnores;
public:
	USensor_Player();
	
	virtual void Init(ACombatUnitPawn* owner) override;
	
	virtual void UpdateAISensing() override;

	ACombatUnitPawn* GetSensedPawn();
};
