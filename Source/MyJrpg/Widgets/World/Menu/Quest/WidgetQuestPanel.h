// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetQuestElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetQuestPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuestPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetQuestElement> m_ClassQuestEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollMainQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_ScrollSubQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMainQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSubQuest;
protected:
	void CreateChilds();
	
	virtual void NativeOnInitialized() override;

	void AddQuest(UQuestLogicBase* q, bool isMain);

	void EraseQuest(UQuestLogicBase* q, bool isMain);
public:
	UFUNCTION()
	void ShowMainQuest();
	UFUNCTION()
	void ShowSubQuest();
};

