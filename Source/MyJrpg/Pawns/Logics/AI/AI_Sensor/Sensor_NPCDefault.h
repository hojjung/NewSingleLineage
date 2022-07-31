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
	DECLARE_DELEGATE_OneParam(FSeePawnDelegate, APawn*);
	DECLARE_DELEGATE_ThreeParams(FHearNoiseDelegate, APawn*, const FVector&, float);
	
protected:
	FVector m_LastSeenLocation;
	
protected:
	virtual void Init(ACombatUnitPawn* owner) override;

	float m_fCurrentTargetDist;



	virtual void UpdateAISensing() override;

	virtual bool CheckDistAndAngle(const ACombatUnitPawn* Other) override;
};
