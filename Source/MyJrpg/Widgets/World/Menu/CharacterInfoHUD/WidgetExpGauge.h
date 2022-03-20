// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Widgets/World/CommonElements/MaterialProgressBar.h"
#include "WidgetExpGauge.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetExpGauge : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextExp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_ExpBar;
public:
	void SetProgressValue(float v);

	virtual bool Initialize() override;

	virtual void NativeOnInitialized() override;

	void UpdateExpBar();
};
