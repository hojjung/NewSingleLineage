// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextText.h"
#include "WidgetCollecStatChild.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "WidgetCollecStatParent.generated.h"

class UCollectionManager;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecStatParent : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetTextText* m_TotalProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_ProgressBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TotalPercent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_Scroll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCollecStatChild> m_ClassStatChild;

	TWeakObjectPtr<UCollectionManager> m_Manager;
	
protected:
	virtual void NativeOnInitialized() override;

	void CreateStats();

public:
	void UpdateTotalProgress();

	void UpdateStats();
};
