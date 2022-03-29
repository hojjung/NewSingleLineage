// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetQuestPrizeElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuestPrizeElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_QuestBaseEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAmount;
	
	FQuestReward m_QuestReward;

protected:
	virtual void NativeOnInitialized() override;

	void OnHoldComplete();
	
public:
	void SetPrize(const FQuestReward& quest_reward);

	void UpdatePrize();
};
