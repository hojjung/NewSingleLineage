// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetHunger.h"
#include "MyJrpg/Widgets/World/Menu/CharacterInfoHUD/WidgetLevelHealthInfo.h"
#include "WidgetStatHealth.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetStatHealth : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetLevelHealthInfo* m_LevelHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_HungerSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetHunger* m_Hunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	bool m_bShowNumber = true;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;
};


