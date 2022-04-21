#pragma once
#include "MyJrpg/MyJrpg.h"
#include "UObject/NoExportTypes.h"
#include "AI_LogicBase.generated.h"

class ACombatUnitPawn;
UCLASS()
class MYJRPG_API UAI_LogicBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ACombatUnitPawn* m_Owner;

protected:
	EKarma GetUnitKarma(const ACombatUnitPawn* Other);

	virtual void OnTargetFocused(const ACombatUnitPawn* Other);

	virtual void OnFocusNeutral();

	virtual void OnFocusFriendly();

	virtual void OnFocusHate();
	
public:
	virtual void Init(ACombatUnitPawn* owner);

	virtual void Tick(float delta);

	bool CheckTargetRange(float rangeSqr);

	bool CheckAngle(float angleEuler);
};
