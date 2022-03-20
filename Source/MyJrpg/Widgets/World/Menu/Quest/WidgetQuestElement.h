// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetQuestHUD.h"
#include "WidgetQuestPrizeElement.h"
#include "WidgetQuestPrizePanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "WidgetQuestElement.generated.h"

/**
 * 전체 퀘스트 목록에 표시되는 위젯
 *	메인 퀘 서브 퀘 따로 존재
 *	처음엔 메인퀘가 효율이 좋은데 메인퀘가 오래걸리는 구간에 도달함
 *	이때부터 개인 활동으로 깨야하고 이때 서브 퀘 수행
 *	
 */
UCLASS()
class MYJRPG_API UWidgetQuestElement : public UWidgetQuestHUDChild
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnComplete;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuestPrizePanel* m_PrizePanel;
	//보상,완성버튼

protected:
	virtual void Init(UQuestLogicBase* quest, bool isMain) override;

	virtual void OnClick() override;
	
public:
	UFUNCTION()
	void OnCompleteQuest();

	virtual void UpdateQuestElement() override;
};
