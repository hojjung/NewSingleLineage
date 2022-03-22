// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetQuestPrizeElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "WidgetQuestPrizePanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuestPrizePanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_WrapRewards;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetQuestPrizeElement> m_ClassQuestPrize;
	UPROPERTY()
	TArray<UWidgetQuestPrizeElement*> m_AryQuestEles;

public:
	void UpdatePrizeElement(const FQuestDataRow& questData);

	void Clear();
};
