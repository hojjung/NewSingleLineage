#pragma once

#include "CoreMinimal.h"
#include "Sensor_LogicBase.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

#include "Sensor_NPCDefault.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API USensor_NPCDefault : public USensor_LogicBase
{
	GENERATED_BODY()

public:
	USensor_NPCDefault();
	
protected:
	FVector m_LastSeenLocation;
	
public:
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);

	virtual void Init(ACombatUnitPawn* owner) override;

	float m_fCurrentTargetDist;

	ACombatUnitPawn* GetSensedPawn();

	virtual void UpdateAISensing() override;

	virtual bool CheckDistAndAngle(const ABaseUnitPawn* Other) override;
};
