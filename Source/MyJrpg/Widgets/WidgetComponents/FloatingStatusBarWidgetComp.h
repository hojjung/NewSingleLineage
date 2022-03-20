// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingStatusBarWidgetComp.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UFloatingStatusBarWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()
	protected:
	UFloatingStatusBarWidgetComp();
	
	public:
	virtual void BeginPlay() override;
	
	void SetHealthPercentage(float HealthPercentage);
};
