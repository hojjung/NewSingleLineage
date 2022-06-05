// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImageText.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ExpGoldWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UExpGoldWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextExp;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* ShowHide;

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	
	void ShowExp(float exp);
};
