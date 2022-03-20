// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "MyWidgetInteractionComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMyWidgetInteractionComponent : public UWidgetInteractionComponent
{
	GENERATED_BODY()

public:
	UMyWidgetInteractionComponent();
	
protected:
	virtual void PressPointerKey(FKey Key) override;
	
	virtual void ReleasePointerKey(FKey Key) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
