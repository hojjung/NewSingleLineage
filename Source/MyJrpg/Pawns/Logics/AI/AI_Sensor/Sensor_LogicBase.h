// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "UObject/NoExportTypes.h"
#include "Sensor_LogicBase.generated.h"

class ACombatUnitPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API USensor_LogicBase : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);
	
public:
	USensor_LogicBase();

protected:
	UPROPERTY()
	ACombatUnitPawn* m_Owner;

	float m_SensingInterval;

	float m_SightRadius;

	float m_PeripheralVisionCosine;

	float m_PeripheralVisionAngle;
	
	FTimerHandle TimerHandle_OnTimer;

public:
	virtual void Init(ACombatUnitPawn* owner);
	
protected:
	void OnTimer();

	void SetTimer(const float TimeDelay);

	virtual void UpdateAISensing();

	virtual bool CheckDistAndAngle(const ACombatUnitPawn* Other);

	void SetPeripheralVisionAngle(const float NewPeripheralVisionAngle);

	ACombatUnitPawn* GetSensedPawn();
public:
	void SetSensingInterval(const float NewSensingInterval);

	void SetSensingUpdatesEnabled(const bool bEnabled);

	bool IsSensorActor(const AActor* Actor) const;

	bool CheckIsHidden(APawn* Pawn) const;

	bool HasLineOfSightTo(const AActor* Other) const;

	FVector GetSensorLocation() const;

	FRotator GetSensorRotation() const;

	AActor* GetSensorActor() const; // Get the actor used as the actual sensor location is derived from this actor.
	
};
