// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Managers/EventStageManager.h"
#include "WidgetEventStageConfirm.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEventStageConfirm : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgEvent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;

	const FEventStageSpec* m_DataRow;

	FVector2D m_BtnPosCached;
	
protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetStageConfirm(const FEventStageSpec& data);

	UFUNCTION()
	void OnLook();
	UFUNCTION()
	void OnClose();
};
