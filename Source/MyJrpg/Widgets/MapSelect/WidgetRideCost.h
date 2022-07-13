// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "WidgetRideCost.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetRideCost : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_CostBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAmount;

protected:
	virtual void NativeOnInitialized() override;

	void UpdateCostText();
};
