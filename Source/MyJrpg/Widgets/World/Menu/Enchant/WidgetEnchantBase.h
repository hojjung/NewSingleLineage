// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetEnchantBase.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEnchantBase : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;
};


