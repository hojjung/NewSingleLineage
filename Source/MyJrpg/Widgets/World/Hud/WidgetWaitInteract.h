// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WidgetWaitInteract.generated.h"

/**
 * m_Bar
 */
UCLASS()
class MYJRPG_API UWidgetWaitInteract : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_Bar; 

	float m_fMaxTimer;

	float m_fTimer;

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void ShowInteract(float time);

	void HideInteract();
};
