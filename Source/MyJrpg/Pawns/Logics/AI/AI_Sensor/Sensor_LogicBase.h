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

protected:
	UPROPERTY()
	ACombatUnitPawn* m_Owner;

	float m_SensingInterval;

	float m_SightRadius;
public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);

public:
	USensor_LogicBase();
	
	virtual void Init(ACombatUnitPawn* owner);
	//
    
	FTimerHandle TimerHandle_OnTimer;
	
protected:
	void OnTimer();

	void SetTimer(const float TimeDelay);

	virtual void UpdateAISensing();

	virtual bool CheckDistAndAngle(const ABaseUnitPawn* Other);

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
