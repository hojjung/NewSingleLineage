// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetQuestHUD.h"
#include "WidgetQuestPrizeElement.h"
#include "WidgetQuestPrizePanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "WidgetQuestAcceptPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuestAcceptPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	FName m_QuestID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnAccept;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextQuestDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuestPrizePanel* m_PrizePanel;
	
protected:
	virtual  void NativeOnInitialized() override;

	void UpdateQuestElement();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	
public:
	void SetQuestAccept(const FName& qId);

	UFUNCTION()
	void OnAcceptQuest();
	UFUNCTION()
	void OnCancel();
};
