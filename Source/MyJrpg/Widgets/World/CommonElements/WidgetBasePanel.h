// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WidgetBasePanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetBasePanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	virtual void ClosePanel();
};
