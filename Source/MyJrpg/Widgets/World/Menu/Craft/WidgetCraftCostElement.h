// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetCraftCostElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCraftCostElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ItemElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCostAmount;

	const FCraftItemCost* m_CraftData;

protected:
	void OnHold(UWidgetBaseElement* ele);
	
public:
	void UpdateCostAmount();
	
	void SetCraftCost(const FCraftItemCost& cost);
};
