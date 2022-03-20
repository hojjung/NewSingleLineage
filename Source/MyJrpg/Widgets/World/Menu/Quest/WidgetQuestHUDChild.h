// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Quest/QuestLogicBase.h"
#include "WidgetQuestHUDChild.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuestHUDChild : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextQuestName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextQuestDesc;

	TWeakObjectPtr<UQuestLogicBase> m_Quest;

	FDelegateHandle m_DeleHandle;

	bool m_bIsMain;

private:
	virtual void OnClick();
	
protected:
	virtual void NativeDestruct() override;

	virtual void UpdateQuestElement();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

public:
	virtual void Init(UQuestLogicBase* quest, bool isMain);
	
	void SetQuest(UQuestLogicBase* quest, bool isMain);

	UQuestLogicBase* GetQuest();
};
