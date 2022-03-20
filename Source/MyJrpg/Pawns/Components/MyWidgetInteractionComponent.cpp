// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWidgetInteractionComponent.h"

#include "MyJrpg/MyJrpg.h"

UMyWidgetInteractionComponent::UMyWidgetInteractionComponent()
{
	bShowDebug = true;

	bEnableHitTesting = true;
}

void UMyWidgetInteractionComponent::PressPointerKey(FKey key)
{
	Super::PressPointerKey(key);
	PRINTF("222");
	if(key.IsTouch())
	{
		PRINTF("ASDDDDDDDD");
	}
}

void UMyWidgetInteractionComponent::ReleasePointerKey(FKey key)
{
	Super::ReleasePointerKey(key);
}

void UMyWidgetInteractionComponent::TickComponent(float delta_time, ELevelTick tick,
	FActorComponentTickFunction* this_tick_function)
{
	UActorComponent::TickComponent(delta_time, tick, this_tick_function);

	SimulatePointerMovement();
}
