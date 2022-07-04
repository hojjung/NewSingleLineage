// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WidgetStatCombat.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetStatCombat : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAtkSpeed;

	float m_OldDmg;

	float m_OldDef;

	float m_OldMSpeed;

	float m_OldAtkSpeed;
	
protected:
	virtual void NativeOnInitialized() override;
	
	void UpdateText();

	void SetTextColorAnim(float& newStat, float& oldStat, UTextBlock* textWant);
};
