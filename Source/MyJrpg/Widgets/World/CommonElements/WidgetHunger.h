// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "WidgetHunger.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetHunger : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarHunger;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextHunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	bool m_bShowNumber = true;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateHunger(float hungerHp);
	
public:
	void ShowNumber(bool b);
	
};

