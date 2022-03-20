// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "WidgetGameOver.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetGameOver : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDeadReason;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnReturnVillage;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION()
	void OnReturnVillage();
	
	void SetKiller(const ACombatUnitPawn* killer);
};
