// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetPawnInfoComp.generated.h"

class ACombatUnitPawn;
class ABaseUnitPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPawnInfoComp : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	UWidgetPawnInfoComp();

public:
	void SetPawnInfo(const ACombatUnitPawn* pawn);
};
