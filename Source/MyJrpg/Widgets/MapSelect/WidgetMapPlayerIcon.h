// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WidgetMapPlayerIcon.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetMapPlayerIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgBG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	//img
	//text
protected:
	void HideText();
	
	void ShowText(const FName& id, float dist);

	void UpdateDuration();

	virtual void NativeOnInitialized() override;
};
