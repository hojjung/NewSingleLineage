// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "WidgetStamina.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetStamina : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_StaminaBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRecharge;

	float m_fTimer;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateRemainTime(float delta);

	void UpdateStaText();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UFUNCTION()
	void OpenRecharge();
};

