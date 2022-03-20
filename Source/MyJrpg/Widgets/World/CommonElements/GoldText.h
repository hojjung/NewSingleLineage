// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageText.h"
#include "Blueprint/UserWidget.h"
#include "GoldText.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UGoldText : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetImageText* m_ImgText;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateGoldText();
};
