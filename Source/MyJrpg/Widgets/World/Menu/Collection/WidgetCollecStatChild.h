// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextText.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Items/Options/OptionBase.h"
#include "WidgetCollecStatChild.generated.h"

struct FItemCollecRow;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecStatChild : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetTextText* m_StatText;

public:
	void SetStatText(TTuple<TSubclassOf<UOptionBase>,TArray<int>> opPair);

	void SetStatText(const FString&& infoText, const FString&& formatText, int v);
};
