// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AlertInfoElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UAlertInfoElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* ShowHide;

protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

public:
	virtual void Show(const FString& str);
	
	void Hide();
};
