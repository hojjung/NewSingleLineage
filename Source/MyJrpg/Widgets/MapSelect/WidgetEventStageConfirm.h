// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Managers/EventStageManager.h"
#include "WidgetEventStageConfirm.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEventStageConfirm : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetStageConfirm(const FEventStageSpec& data);
};
